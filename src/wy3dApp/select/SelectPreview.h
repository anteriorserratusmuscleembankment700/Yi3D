///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024-2026 Wang Yao <wangyao1052@163.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3DAPP_SELECT_PREVIEW_H
#define WY3DAPP_SELECT_PREVIEW_H

#include <memory>
#include <wyapSelection.h>

class SelectPreview
{
public:
    explicit SelectPreview(const wyap::Selection& selection);
    ~SelectPreview();

    inline const wyap::Selection& getSelection() const
    {
        return _selection;
    }

    inline bool isEqual(const wyap::Selection& selection) const
    {
        return !(_selection < selection) && !(selection < _selection);
    }

private:
    void showSelection(const wyap::Selection& selection, bool value);

private:
    wyap::Selection _selection;
};
typedef std::shared_ptr<SelectPreview> SelectPreviewSPtr;

#endif // WY3DAPP_SELECT_PREVIEW_H