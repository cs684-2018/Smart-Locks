
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: MainActivity.java
* Theme: SmartLock
* Functions: onCreate(Bundle), AuthenticationHandler(), onSuccess(CognitoUserSession, CognitoDevice),
*            getAuthenticationDetails(AuthenticationContinuation, String), getMFACode(MultiFactorAuthenticationContinuation),
*           authenticationChallenge(ChallengeContinuation),onFailure(Exception), onBackPressed()
* Global Variables: button, Username, Password, username, password, awesomeValidation, doubleBackToExitPressedOnce
*
*/



package com.example.user.smartlock;

import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoDevice;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUser;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUserSession;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.continuations.AuthenticationContinuation;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.continuations.AuthenticationDetails;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.continuations.ChallengeContinuation;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.continuations.MultiFactorAuthenticationContinuation;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.handlers.AuthenticationHandler;

import com.basgeekball.awesomevalidation.AwesomeValidation;
import com.basgeekball.awesomevalidation.ValidationStyle;

import java.util.Objects;

public class MainActivity extends AppCompatActivity{

    Button button;
    private String Username,Password;
    private EditText username,password;
    private AwesomeValidation awesomeValidation;
    boolean doubleBackToExitPressedOnce = false;

    /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file and also created a logic when a submit button is clicked.
* Example Call: onCreate(Bundle);
*
*/


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_main);

        //AppHelper.init(getApplicationContext());
        AWSMobileClient.getInstance().initialize(this).execute();


        username= (EditText)findViewById(R.id.editText);
        password= (EditText) findViewById(R.id.editText2);

        awesomeValidation = new AwesomeValidation(ValidationStyle.BASIC);


        AppHelper.init(getApplicationContext());
        CognitoUser user = AppHelper.getPool().getCurrentUser();

        Log.d("msg", user.toString());

        Username = user.getUserId();
        if(Username != null) {
            //AppHelper.setUser(username);
            //inUsername.setText(user.getUserId());
            user.getSessionInBackground(authenticationHandler);
        }



        //Awesome validation

        awesomeValidation.addValidation(this, R.id.editText, ".{1,}", R.string.usernameerror);
        awesomeValidation.addValidation(this, R.id.editText2, ".{6,}", R.string.passworderror);


        button = (Button) findViewById(R.id.login);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Username = username.getText().toString();

                if (awesomeValidation.validate()) {

                AppHelper.getPool().getUser(Username).getSessionInBackground(authenticationHandler);
               }

            }
        });
    }

    /*
* Function Name: AuthenticationHandler
* Input: None
* Output: None
* Logic: It checks an entry in the amazon cognito  user pool for a particular user.
* If it is present then it calls  onSuccess() otherwise it calls onFailure()
* Example Call: AuthenticationHandler();
*
*/
    AuthenticationHandler authenticationHandler = new AuthenticationHandler() {


        /*
* Function Name: onSuccess
* Input: cognitoUserSession -> CognitoUserSession which stores the current session id, device ->  CognitoDevice which stores the cognito device Id
* Output:
* Logic: It transfers the user from current Activity to HomeActivity
* Example Call: onSuccess(cognitoUserSession,device);
*
*
*/
        @Override
        public void onSuccess(CognitoUserSession cognitoUserSession, CognitoDevice device) {

            AppHelper.setCurrSession(cognitoUserSession);
            AppHelper.newDevice(device);

            Log.d("msg", "success");
            Log.d("msg", Username);

            if (Objects.equals(Username, "embedded")) {
                Intent userActivity = new Intent(MainActivity.this, HomeActivity.class);
                //userActivity.putExtra("name", username);
                startActivityForResult(userActivity, 4);
            }
            else
            {
                Intent userActivity = new Intent(MainActivity.this, HomeActivityOther.class);
                //userActivity.putExtra("name", username);
                startActivityForResult(userActivity, 4);
            }
        }

        /*
* Function Name: getAuthenticationDetails
* Input: authenticationContinuation -> AuthenticationContinuation it stores authenticationContinuation , username -> String which stores the username
* Output: None
* Logic: gets the authentication details related to a particular user Using an inbuild function
* Example Call: getAuthenticationDetails(authenticationContinuation,username);
*
*/

        @Override
        public void getAuthenticationDetails(AuthenticationContinuation authenticationContinuation, String username) {

            if(username != null) {
                Username = username;

            }

            // The API needs user sign-in credentials to continue
            Password=password.getText().toString();

            AuthenticationDetails authenticationDetails = new AuthenticationDetails(Username, Password, null);

            // Pass the user sign-in credentials to the continuation
            authenticationContinuation.setAuthenticationDetails(authenticationDetails);

            // Allow the sign-in to continue
            authenticationContinuation.continueTask();

        }

        /*
* Function Name: getMFACode
* Input: multiFactorAuthenticationContinuation -> MultiFactorAuthenticationContinuation which the stores the verification code entered by user
* Output: None
* Logic: calls the multiFactorAuthenticationContinuation.continueTask() function
* Example Call: getMFACode(MultiFactorAuthenticationContinuation multiFactorAuthenticationContinuation);
*
*/

        @Override
        public void getMFACode(MultiFactorAuthenticationContinuation multiFactorAuthenticationContinuation) {

            // Multi-factor authentication is required; get the verification code from user
           // multiFactorAuthenticationContinuation.setMfaCode(mfaVerificationCode);
            // Allow the sign-in process to continue
            Log.d("msg","mfa");
            multiFactorAuthenticationContinuation.continueTask();
        }

        /*
* Function Name: authenticationChallenge
* Input: continuation -> ChallengeContinuation
* Output: None
* Logic: None its not used but has to be coded
* Example Call: authenticationChallenge(ChallengeContinuation);
*
*/

        @Override
        public void authenticationChallenge(ChallengeContinuation continuation) {

            Log.d("msg","challenge");
        }

        /*
* Function Name: onFailure
* Input: e -> Exception which stores the error if occured
* Output: None
* Logic: Prints the error message
* Example Call: onFailure(e);
*
*/

        @Override
        public void onFailure(Exception e) {
            Log.d("msg","exception");
            Log.d("msg",e.getMessage());

        }

    };

    /*
* Function Name: onBackPressed
* Input: None
* Output: None
* Logic: It checks how many times back button is pressed by a user,
* if its twice with a interval less than 2 seconds between two back press then it closes the app
* or it prompts the message "Please click BACK again to exit"
*
* Example Call: onBackPressed());
*
*/

    public void onBackPressed() {
        if (doubleBackToExitPressedOnce) {
            super.onBackPressed();
            return;
        }

        this.doubleBackToExitPressedOnce = true;
        Toast.makeText(this, "Please click BACK again to exit", Toast.LENGTH_SHORT).show();

        new Handler().postDelayed(new Runnable() {

            @Override
            public void run() {
                doubleBackToExitPressedOnce=false;
            }
        }, 2000);
    }
}
