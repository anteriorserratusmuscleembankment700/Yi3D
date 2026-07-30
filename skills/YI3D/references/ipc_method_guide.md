# IPC Method Guide (for Agent)

本文档基于当前 `src/wy3dApp/application/IpcServer.cpp` 实现，描述 Agent 通过 `scripts/IpcClient.py` 发送命令时需要关注的行为。

## 1. Agent 需要做什么

1. 先执行 `ping` 做连通性探测：
   `python skills/YI3D/scripts/IpcClient.py --method ping`。
2. 若 `ping` 无响应、超时或连接失败，提醒用户先打开 `YI3D.exe`（优先 `out/Release/YI3D.exe`，不存在则 `out/Debug/YI3D.exe`），然后再次 `ping`。
3. 连通后再选择 method：`ping` / `script` / `command`。
4. 填写 `--argument`（按 method 要求）。
5. 调用 `IpcClient.py`。
6. 按返回内容判断成功或失败。

调用模板：

```bash
python skills/YI3D/scripts/IpcClient.py --method <method> --argument <arg>
```

## 2. 返回格式与判断

`IpcClient.py` 解析服务端响应后，返回 Python 字典：

- 成功：`{"ok": True, "data": "..."}`
- 失败：`{"ok": False, "error": "..."}`

服务端文本格式为：

- 成功：`OK|<message>`
- 失败：`ERROR|<error>`

## 3. 命令定义（按当前实现）

| 命令 | argument | 服务端行为 | 典型返回 |
|---|---|---|---|
| `ping` | 空字符串 | 立即返回连通性结果 | `OK|pong` |
| `script` | 脚本路径 | 校验后异步执行脚本，执行完成后回包 | `OK|NoError` 或 `ERROR|RunScriptError` 等 |
| `command` | GUI 命令名（见 `./command_names.md`） | 投递 `ExecuteCommandEvent` | 见下方“4.2 command 现状” |

说明：请求 JSON 的命令字段名是历史拼写 `commnad`，由 `IpcClient.py` 内部处理，Agent 不要手工拼 JSON。

## 4. 各命令细节

### 4.1 script

`script` 在服务端会执行以下校验：

- `argument` 不能为空，否则 `ERROR|MissingArgumentField`
- 路径不存在则 `ERROR|file not found: <path>`
- 事件接收器未配置则 `ERROR|EventReceiverNotConfigured`

校验通过后，服务端进入等待执行结果状态，再由主线程回调 `completeCurrentRequest(...)` 返回最终结果：

- 成功：`OK|NoError`
- 失败：`ERROR|PythonLibraryNotFound` / `LoadPythonLibraryFailed` / `InvalidPythonLibrary` / `PythonScriptFileNotFound` / `OpenPythonScriptFileFailed` / `RunScriptError` / `UnknownError`

### 4.2 command 现状

`command` 的 `argument` 需要传入 GUI 命令字符串，完整清单见：

- `./command_names.md`

示例（触发撤销命令）：

```bash
python skills/YI3D/scripts/IpcClient.py --method command --argument "Undo"
```

当前实现中，`onCmdCommand(...)` 会把命令投递为 `ExecuteCommandEvent`，但在 `wy3dQApplication::notify(...)` 的 `ExecuteCommandEvent` 分支里没有调用 `_ipc->completeCurrentRequest(...)`。

这意味着：

- `command` 命令可能拿不到标准 `OK|...` / `ERROR|...` 回包。
- 使用 `IpcClient.py` 调用时，可能表现为 socket 超时异常，而不是解析后的 `{"ok": ...}` 结果。

在服务端补齐回包逻辑前，Agent 应谨慎使用 `command`，不要把它当作“必有回包”的命令。

## 5. 常见错误

- `Busy`：服务端忙（已有连接或正在等待上一个异步结果），稍后重试。
- `MissingArgumentField`：缺少 `argument`。
- `file not found: ...`：脚本路径无效。
- `RunScriptError`：脚本运行失败。
- `UnsupportedCommand:<name>`：命令名不支持。

## 6. Agent 不需要关心的内容

- IPC 帧头二进制细节（magic/version/msg_type/payload_len）。
- 字节序、分帧与底层 socket 读写。

这些由 `IpcClient.py` 与服务端内部实现负责。

