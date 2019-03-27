// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include "common.h"

#include "TabView.g.h"
#include "TabView.properties.h"

class TabView :
    public ReferenceTracker<TabView, winrt::implementation::TabViewT>,
    public TabViewProperties
{

public:
    TabView();
    ~TabView() {}

    // IFrameworkElement
    void OnApplyTemplate();

    void OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);

private:
    void OnLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnSelectionChanged(const winrt::IInspectable& sender, const winrt::SelectionChangedEventArgs& args);
    void OnSizeChanged(const winrt::IInspectable& sender, const winrt::SizeChangedEventArgs& args);

    void UpdateTabWidths();

    tracker_ref<winrt::ContentPresenter> m_tabContentPresenter{ this };
    tracker_ref<winrt::FrameworkElement> m_scrollViewer{ this };

    winrt::ListViewBase::Loaded_revoker m_loadedRevoker{};
    winrt::ListViewBase::SizeChanged_revoker m_sizeChangedRevoker{};
    winrt::ListViewBase::SelectionChanged_revoker m_selectionChangedRevoker{};
};
