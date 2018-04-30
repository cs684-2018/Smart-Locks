
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: Logout.java
* Theme: SmartLock
* Functions: onCreate(Bundle)
* Global Variables: None
*/

package com.example.user.smartlock;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUser;

/**
 * Created by user on 19/3/18.
 */

public class Logout extends AppCompatActivity {


    /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It gets the current user details and signout the user.
* Example Call: onCreate(Bundle);
*
*/


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        CognitoUser user = AppHelper.getPool().getCurrentUser();

        user.signOut();

        Intent c = new Intent(Logout.this, MainActivity.class);
        startActivity(c);
    }
}
