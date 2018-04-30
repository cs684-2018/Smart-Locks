
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: ChangePassword.java
* Theme: SmartLock
* Functions: onCreate(Bundle), GenericHandler(), onSuccess(),
* onFailure(Exception)
* Global Variables: oldpassord, newpassword, toolbar, change
*/


package com.example.user.smartlock;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toolbar;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUser;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.handlers.GenericHandler;


/**
 * Created by user on 19/4/18.
 */

public class ChangePassword extends AppCompatActivity {

    private EditText oldpassord,newpassword;
    Toolbar toolbar;
    Button change;

       /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file,
* It creates a logic for change button
* Example Call: onCreate(Bundle);
*
*/

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_changepassword);

        oldpassord = (EditText) findViewById(R.id.oldpassword);
        newpassword = (EditText) findViewById(R.id.newpassword);
        final CognitoUser user = AppHelper.getPool().getCurrentUser();

        change=(Button) findViewById(R.id.change);




        change.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {


                user.changePasswordInBackground(oldpassord.getText().toString(), newpassword.getText().toString(), handler);

            }
        });

    }


    /*
* Function Name: GenericHandler
* Input: None
* Output: None
* Logic: It performs the operation for changing the password
* Example Call: GenericHandler();
*
*/

    GenericHandler handler = new GenericHandler() {



        /*
    * Function Name: onSuccess
    * Input: None
    * Output: None
    * Logic: It shifts the user activity from current activity to HomeActivity
    * Example Call: onSuccess();
    *
    */

        @Override
        public void onSuccess() {
           Log.d("msg","s");
            Intent userActivity = new Intent(ChangePassword.this, HomeActivity.class);
            //userActivity.putExtra("name", username);
            startActivityForResult(userActivity, 4);
        }


        /*
    * Function Name: onFailure
    * Input: None
    * Output: None
    * Logic: It prints the error message
    * Example Call: onFailure(Exception);
    *
    */
        @Override
        public void onFailure(Exception exception) {
            // Password change failed, probe exception for details
            Log.d("msg","f");
            Log.d("msg",exception.getMessage());
        }
    };

}
