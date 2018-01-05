/*
 * Copyright (C) 2015 Opersys inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

var Binder = require("jsbinder");

function testParcel() {
    var p = new Binder.Parcel();

    console.log("TEST PARCEL:");

    p.writeInterfaceToken("test")    
    p.writeInt32(10);
    console.log("Current data position: " + p.dataPosition());
    p.setDataPosition(0);
    console.log("New data position: " + p.dataPosition());
    console.log(p.readInt32());

    p.setDataPosition(0);
    p.writeString("blarg");
    console.log("Current data position: " + p.dataPosition());
    p.setDataPosition(0);
    console.log("New data position:" + p.dataPosition());
    console.log(p.readString());
    console.log("New data position:" + p.dataPosition());
}

function testServiceCallWithParcel() {
    console.log("SERVICE CALL WITH PARCEL:");

    var sm = new Binder.ServiceManager();
    console.log("Got ServiceManager object.")
    
    var dm = sm.getService("display");
    console.log("Got 'display' service");
    
    var pDm = new Binder.Parcel();

    console.log("DisplayManager.getDisplay");

    console.log("Writing Interface Token");
    pDm.writeInterfaceToken("android.hardware.display.IDisplayManager");

    console.log("Writing Call Parameter");
    pDm.writeInt32(0);

    console.log("Transacting");
    var np = dm.transact(1, pDm);

    console.log("Returned data size: " + np.dataSize());
}

function testPowerManagerCalls() {
    console.log("SERVICE CALL ON POWERMANAGER:");
    
    var sm = new Binder.ServiceManager();
    console.log("Got ServiceManager object.");
    
    var pm = sm.getService("power");
    console.log("Got 'power' service");
    
    var pPm = new Binder.Parcel();

    console.log("PowerManager.isScreenOn");
    pPm.writeInterfaceToken("android.os.IPowerManager");
    var np = pm.transact(11, pPm);

    console.log("Returned data size: " + np.dataSize());
    np.readExceptionCode();
    console.log("isScreenOn: " + np.readInt32());
    console.log(pm.getInterface());
}

try {
    console.log("#### TESTING PARCEL #####");
    testParcel();
    console.log("**** WIN ****");
} catch (e) {
    console.log(e);
    console.log("!!!! FAILED !!!!");
}

try {
    console.log("#### TEST SERVICE CALL WITH PARCELS ####");
    testServiceCallWithParcel();
    console.log("**** WIN ****");
} catch (e) {
    console.log(e);
    console.log("!!!! FAILED !!!!");
}

try {
    console.log("#### TEST POWER MANAGER CALLS ####");
    testPowerManagerCalls();
    console.log("**** WIN ****");
} catch (e) {
    console.log(e);
    console.log("!!!! FAILED !!!!");
}
    
