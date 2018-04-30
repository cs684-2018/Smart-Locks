

/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: AWSProvider.java
* Theme: SmartLock
* Functions: getInstance(), initialize(), AWSProvider(Context), getContext()
* getConfiguration(), getIdentityManager(), getPinpointManager()
* Global Variables: None
*/

package com.example.user.smartlock;

/**
 * Created by user on 25/3/18.
 */

import android.content.Context;
import com.amazonaws.auth.AWSCredentialsProvider;
import com.amazonaws.mobile.auth.core.IdentityManager;
import com.amazonaws.mobile.auth.userpools.CognitoUserPoolsSignInProvider;
import com.amazonaws.mobile.config.AWSConfiguration;
import com.amazonaws.mobileconnectors.pinpoint.PinpointConfiguration;
import com.amazonaws.mobileconnectors.pinpoint.PinpointManager;

public class AWSProvider {
    private static AWSProvider instance = null;
    private Context context;
    private AWSConfiguration awsConfiguration;
    private PinpointManager pinpointManager;

         /*
* Function Name: getInstance
* Input: None
* Output: AWSProvider
* Logic: It returns a value
* and sets up a connection with AWS iot
* Example Call: getInstance();
*
*/


    public static AWSProvider getInstance() {
        return instance;
    }

         /*
* Function Name: initialize
* Input: context -> Context
* Output: None
* Logic: It sets the value of a variable
* Example Call: initialize(Context);
*
*/

    public static void initialize(Context context) {
        if (instance == null) {
            instance = new AWSProvider(context);
        }
    }

    /*
* Function Name: AWSProvider
* Input: context -> Context
* Output: None
* Logic: It sets the value of a variable
* Example Call: AWSProvider(Context);
*
*/

    private AWSProvider(Context context) {
        this.context = context;
        this.awsConfiguration = new AWSConfiguration(context);

        IdentityManager identityManager = new IdentityManager(context, awsConfiguration);
        IdentityManager.setDefaultIdentityManager(identityManager);
        identityManager.addSignInProvider(CognitoUserPoolsSignInProvider.class);
    }

       /*
* Function Name: getContext
* Input: None
* Output: Context
* Logic: It returns the value
* Example Call: getContext();
*
*/


    public Context getContext() {
        return this.context;
    }

       /*
* Function Name: getConfiguration
* Input: None
* Output: AWSConfiguration
* Logic: It returns the value
* Example Call: getConfiguration();
*
*/


    public AWSConfiguration getConfiguration() {
        return this.awsConfiguration;
    }

      /*
* Function Name: getIdentityManager
* Input: None
* Output: IdentityManager
* Logic: It returns the value
* Example Call: getIdentityManager();
*
*/

    public IdentityManager getIdentityManager() {
        return IdentityManager.getDefaultIdentityManager();
    }

     /*
* Function Name: getPinpointManager
* Input: None
* Output: PinpointManager
* Logic: It returns the value of pinpointManager
* Example Call: getPinpointManager();
*
*/


    public PinpointManager getPinpointManager() {
        if (pinpointManager == null) {
            final AWSCredentialsProvider cp = getIdentityManager().getCredentialsProvider();
            PinpointConfiguration config = new PinpointConfiguration(
                    getContext(), cp, getConfiguration());
            pinpointManager = new PinpointManager(config);
        }
        return pinpointManager;
    }
}