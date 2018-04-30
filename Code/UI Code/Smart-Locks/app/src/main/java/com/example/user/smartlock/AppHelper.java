

/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: AppHelper.java
* Theme: SmartLock
* Functions: init(Context), newDevice(CognitoDevice), setThisDevice(CognitoDevice),
* setCurrSession(CognitoUserSession), getPool()
* Global Variables: userPool, thisDevice, newDevice, currSession
*/

package com.example.user.smartlock;

import android.content.Context;

import com.amazonaws.ClientConfiguration;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoDevice;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUserPool;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUserSession;
import com.amazonaws.regions.Regions;


/**
 * Created by user on 31/3/18.
 */

public class AppHelper {

    private static CognitoUserPool userPool;
    private static CognitoDevice thisDevice;
    private static CognitoUserSession currSession;
    private static CognitoDevice newDevice;

        /*
* Function Name: init
* Input: context -> Context
* Output: None
* Logic: It sets up a connection with CognitoUserPool of aws
* Example Call: init(Context);
*
*/

    public static void init(Context context) {

        if (userPool == null) {

            ClientConfiguration clientConfiguration = new ClientConfiguration();


            // Create a user pool with default ClientConfiguration
            userPool = new CognitoUserPool(context, "us-east-2_l4mIhcfgn", "6c5ih5r9g3rh14bt6paaflfsn0", "2p6en9fv7poh22eakdhp7v2963brcn64rdvouqh7qo8nfpaucje",Regions.US_EAST_2);


        }

    }


    /*
* Function Name: newDevice
* Input: None
* Output: None
* Logic: It sets up a Value of newDevice to device
* Example Call: newDevice(CognitoDevice);
*
*/

    public static void newDevice(CognitoDevice device) {
        newDevice = device;
    }

    /*
* Function Name: setThisDevice
* Input: None
* Output: None
* Logic: It sets up a Value of thisDevice to device
* Example Call: setThisDevice(CognitoDevice);
*
*/
    public static void setThisDevice(CognitoDevice device) {
        thisDevice = device;
    }

        /*
* Function Name: setCurrSession
* Input: None
* Output: None
* Logic: It sets up a Value of currSession to session
* Example Call: setCurrSession(CognitoUserSession);
*
*/

    public static void setCurrSession(CognitoUserSession session) {
        currSession = session;
    }

    /*
* Function Name: getPool
* Input: None
* Output: CognitoUserPool
* Logic: Just return a value
* Example Call: getPool();
*
*/
    public static CognitoUserPool getPool() {
        return userPool;
    }
}
