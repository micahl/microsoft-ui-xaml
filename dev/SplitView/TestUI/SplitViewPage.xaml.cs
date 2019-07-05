// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MUXControlsTestApp
{
    public sealed partial class SplitViewPage : TestPage
    {
        public SplitViewPage()
        {
            this.InitializeComponent();
            this.SizeChanged += SplitViewPage_SizeChanged;
            TestSplitView.PaneOpening += TestSplitView_PaneOpening;
            TestSplitView.PaneClosing += TestSplitView_PaneClosing;
        }

        private void TestSplitView_PaneClosing(SplitView sender, SplitViewPaneClosingEventArgs args)
        {
            OpenStateText.Text = "Closing";
        }

        private void TestSplitView_PaneOpening(SplitView sender, object args)
        {
            OpenStateText.Text = "Opening";
        }

        private void SplitViewPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            var width = e.NewSize.Width;
            if (width < 600)
            {
                TestSplitView.Pane.Visibility = Visibility.Collapsed;
                TestSplitView.DisplayMode = SplitViewDisplayMode.CompactOverlay;
            }
            else
            {
                TestSplitView.DisplayMode = SplitViewDisplayMode.CompactInline;
                TestSplitView.IsPaneOpen = true;
            }
            WidthText.Text = width.ToString();
            if (TestSplitView.DisplayMode == SplitViewDisplayMode.CompactOverlay)
            {
                if (TestSplitView.IsPaneOpen) TestSplitView.IsPaneOpen = false;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            TestSplitView.IsPaneOpen = !TestSplitView.IsPaneOpen;
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (TestSplitView.DisplayMode == SplitViewDisplayMode.CompactInline)
            {
                TestSplitView.DisplayMode = SplitViewDisplayMode.CompactOverlay;
            }
            else
            {
                TestSplitView.DisplayMode = SplitViewDisplayMode.CompactInline;
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            OpenStateText.Text = TestSplitView.IsPaneOpen.ToString();
        }
    }
}
