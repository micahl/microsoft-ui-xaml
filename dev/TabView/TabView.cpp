// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "TabView.h"
#include "DoubleUtil.h"
#include "RuntimeProfiler.h"
#include "ResourceAccessor.h"
#include "SharedHelpers.h"

static constexpr double c_tabMaximumWidth = 200.0;
static constexpr double c_tabMinimumWidth = 48.0;

TabView::TabView()
{
    __RP_Marker_ClassById(RuntimeProfiler::ProfId_TabView);

    SetDefaultStyleKey(this);
}

void TabView::OnApplyTemplate()
{
    winrt::IControlProtected controlProtected{ *this };

    m_tabContentPresenter.set(GetTemplateChildT<winrt::ContentPresenter>(L"TabContentPresenter", controlProtected));
    m_scrollViewer.set(GetTemplateChildT<winrt::FxScrollViewer>(L"ScrollViewer", controlProtected));

    //### do I need a revoker when listening to my own event....??
    m_loadedRevoker = Loaded(winrt::auto_revoke, { this, &TabView::OnLoaded });
    m_selectionChangedRevoker = SelectionChanged(winrt::auto_revoke, { this, &TabView::OnSelectionChanged });
    m_sizeChangedRevoker = SizeChanged(winrt::auto_revoke, { this, &TabView::OnSizeChanged });
}

void TabView::OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args)
{
    winrt::IDependencyProperty property = args.Property();

    if (property == s_TabWidthModeProperty)
    {
        UpdateTabWidths();
    }
}

void TabView::OnLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args)
{
    winrt::IControlProtected controlProtected{ *this };

    // ### yeah probably need to do this on scrollviewer load, not this on loaded, anyway.
    if (auto scrollViewer = m_scrollViewer.get())
    {
        m_scrollDecreaseButton.set(SharedHelpers::FindInVisualTreeByName(scrollViewer, L"ScrollDecreaseButton").as<winrt::RepeatButton>());
        if (auto decreaseButton = m_scrollDecreaseButton.get())
        {
            m_scrollDecreaseClickRevoker = decreaseButton.Click(winrt::auto_revoke, { this, &TabView::OnScrollDecreaseClick });
        }

        m_scrollIncreaseButton.set(SharedHelpers::FindInVisualTreeByName(scrollViewer, L"ScrollIncreaseButton").as<winrt::RepeatButton>());
        if (auto increaseButton = m_scrollIncreaseButton.get())
        {
            m_scrollIncreaseClickRevoker = increaseButton.Click(winrt::auto_revoke, { this, &TabView::OnScrollIncreaseClick });
        }
    }

    UpdateTabWidths();
}

void TabView::OnSizeChanged(const winrt::IInspectable& sender, const winrt::SizeChangedEventArgs& args)
{
    UpdateTabWidths();
}


void TabView::OnSelectionChanged(const winrt::IInspectable& sender, const winrt::SelectionChangedEventArgs& args)
{
    if (auto tabContentPresenter = m_tabContentPresenter.get())
    {
        if (!SelectedItem())
        {
            tabContentPresenter.Content(nullptr);
            tabContentPresenter.ContentTemplate(nullptr);
        }
        else
        {
            auto container = ContainerFromItem(SelectedItem()).as<winrt::ListViewItem>();
            if (container)
            {
                //if (ContainerFromItem(SelectedItem) is TabViewItem container)
                //{
                tabContentPresenter.Content(container.Content());
                tabContentPresenter.ContentTemplate(container.ContentTemplate());
                //}
            }
        }
    }
}

void TabView::OnScrollDecreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args)
{
    if (auto scrollViewer = m_scrollViewer.get())
    {
        scrollViewer.ChangeView(std::max(0.0, scrollViewer.HorizontalOffset() - 50.0), nullptr, nullptr); //### magic numbers
    }
}

void TabView::OnScrollIncreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args)
{
    if (auto scrollViewer = m_scrollViewer.get())
    {
        scrollViewer.ChangeView(std::min(scrollViewer.ScrollableWidth(), scrollViewer.HorizontalOffset() + 50.0), nullptr, nullptr);
    }
}

void TabView::UpdateTabWidths()
{
    // ### also call this on size changed
    if (TabWidthMode() == winrt::TabViewWidthMode::SizeToContent)
    {
        for (int i = 0; i < (int)(Items().Size()); i++)
        {
            auto container = ContainerFromItem(Items().GetAt(i)).as<winrt::ListViewItem>();
            if (container)
            {
                container.Width(DoubleUtil::NaN);
            }
        }
    }
    else
    {
        //### need function for this -- probably somewhere in common
        double maxTabWidth = c_tabMaximumWidth;
        auto resourceName = box_value(L"TabViewItemMaxWidth");
        if (winrt::Application::Current().Resources().HasKey(resourceName))
        {
            if (auto lookup = winrt::Application::Current().Resources().Lookup(resourceName))
            {
                maxTabWidth = unbox_value<double>(lookup);
            }
        }

        double minTabWidth = c_tabMinimumWidth;
        resourceName = box_value(L"TabViewItemMinWidth");
        if (winrt::Application::Current().Resources().HasKey(resourceName))
        {
            if (auto lookup = winrt::Application::Current().Resources().Lookup(resourceName))
            {
                minTabWidth = unbox_value<double>(lookup);
            }
        }

        double tabWidth = maxTabWidth;
        if (auto scrollViewer = m_scrollViewer.get())
        {
            double padding = Padding().Left + Padding().Right;

            // ### probably some rounding error here....?
            double tabWidthForScroller = (scrollViewer.ActualWidth() - padding) / (double)(Items().Size()); //### ExtentWidth() didn't work??
            tabWidth = std::min(std::max(tabWidthForScroller, minTabWidth), maxTabWidth);

            auto decreaseButton = m_scrollDecreaseButton.get();
            auto increaseButton = m_scrollIncreaseButton.get();
            if (decreaseButton && increaseButton)
            {
                // ### maybe this is right, maybe there's something else to check...
                if (tabWidthForScroller < tabWidth)
                {
                    decreaseButton.Visibility(winrt::Visibility::Visible);
                    increaseButton.Visibility(winrt::Visibility::Visible);
                }
                else
                {
                    decreaseButton.Visibility(winrt::Visibility::Collapsed);
                    increaseButton.Visibility(winrt::Visibility::Collapsed);
                }
            }
        }

        for (int i = 0; i < (int)(Items().Size()); i++)
        {
            auto container = ContainerFromItem(Items().GetAt(i)).as<winrt::ListViewItem>();
            if (container)
            {
                container.Width(tabWidth);
            }
        }
    }
}
