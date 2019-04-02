// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;

namespace MUXControlsTestApp
{
    class TestInventory
    {
        static TestInventory()
        {
            Tests = new List<TestDeclaration>();

#if !BUILD_LEAN_MUX_FOR_THE_STORE_APP
            Tests.Add(new TestDeclaration("Leak Tests", typeof(LeakTestPage)));
            Tests.Add(new TestDeclaration("TeachingTip Tests", typeof(TeachingTipPage)));
#endif

        }

        public static List<TestDeclaration> Tests { get; private set; }
    }
}

