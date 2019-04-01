// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "TabViewItem.h"
#include "RuntimeProfiler.h"
#include "ResourceAccessor.h"

TabViewItem::TabViewItem()
{
    __RP_Marker_ClassById(RuntimeProfiler::ProfId_TabViewItem);

    SetDefaultStyleKey(this);
}

void TabViewItem::OnApplyTemplate()
{
    winrt::IControlProtected controlProtected{ *this };

    m_closeButton.set(GetTemplateChildT<winrt::Button>(L"CloseButton", controlProtected));

    //### do I need a revoker when listening to my own event....??
    m_loadedRevoker = Loaded(winrt::auto_revoke, { this, &TabViewItem::OnLoaded });

    //### actually pretty sure I don't need to do this -- can't I just do this from PropertyChanged
    m_IsSelectedChangedRevoker = RegisterPropertyChanged(*this, winrt::SelectorItem::IsSelectedProperty(), { this, &TabViewItem::OnIsSelectedChanged });
}

void TabViewItem::OnLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args)
{
    UpdateCloseButton();
}

void TabViewItem::UpdateCloseButton()
{
    if (auto closeButton = m_closeButton.get())
    {
        closeButton.Visibility(IsSelected() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
    }
}

void TabViewItem::OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args)
{
    winrt::IDependencyProperty property = args.Property();
    
    // TODO: Implement
}

void TabViewItem::OnIsSelectedChanged(const winrt::DependencyObject& sender, const winrt::DependencyProperty& args)
{
    UpdateCloseButton();
}
