// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Markup;
using Windows.UI;
using System.Windows.Input;

#if !BUILD_WINDOWS
using TabView = Microsoft.UI.Xaml.Controls.TabView;
using TabViewTabClosingEventArgs = Microsoft.UI.Xaml.Controls.TabViewTabClosingEventArgs;
#endif

namespace MUXControlsTestApp
{
    public sealed partial class TabViewPage : TestPage
    {
        public TabViewPage()
        {
            this.InitializeComponent();
        }

        public void CanCloseCheckBox_CheckChanged(object sender, RoutedEventArgs e)
        {
            Tabs.CanCloseTabs = (bool)CanCloseCheckBox.IsChecked;
        }

        public void IsClosableCheckBox_CheckChanged(object sender, RoutedEventArgs e)
        {
            FirstTab.IsClosable = (bool)IsClosableCheckBox.IsChecked;
        }
    }
}
