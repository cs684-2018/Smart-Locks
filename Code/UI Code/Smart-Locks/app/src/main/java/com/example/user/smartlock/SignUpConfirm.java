

/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: SignUpConfirm.java
* Theme: SmartLock
* Functions: onCreate(Bundle), init(), GenericHandler(), onSuccess(),
*            onFailure(Exception), createUser()
* Global Variables: userName, emailid, username, confCode, confirm, dynamoDBMapper
*/


package com.example.user.smartlock;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBMapper;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.handlers.GenericHandler;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClient;
import java.text.DateFormat;
import java.util.Date;

/**
 * Created by user on 2/4/18.
 */

public class SignUpConfirm extends AppCompatActivity {

    private String userName,emailid;
    private EditText username;
    private EditText confCode;
    Button confirm;
    DynamoDBMapper dynamoDBMapper;


    /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file
* and sets up a connection with AWS and calls an init() function.
* Example Call: onCreate(Bundle);
*
*/


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_sign_up_confirm);


        // Instantiate a AmazonDynamoDBMapperClient
        AmazonDynamoDBClient dynamoDBClient = new AmazonDynamoDBClient(AWSMobileClient.getInstance().getCredentialsProvider());
        this.dynamoDBMapper = DynamoDBMapper.builder()
                .dynamoDBClient(dynamoDBClient)
                .awsConfiguration(AWSMobileClient.getInstance().getConfiguration())
                .build();

        init();
    }

       /*
* Function Name: init
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It creates a logic for the confirm button once it gets clicked
* Example Call: init();
*
*/

    private void init()
    {
        String confirmCode = null;
        Bundle extras = getIntent().getExtras();
        if (extras !=null) {

            userName = extras.getString("name");

            emailid = extras.getString("email");
            username = (EditText) findViewById(R.id.editText3);
            username.setText(userName);

            confCode = (EditText) findViewById(R.id.editText4);
            confCode.requestFocus();

            confirm= (Button) findViewById(R.id.confirm);
        }


        confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                userName = username.getText().toString();
                String confirmCode = confCode.getText().toString();
                AppHelper.getPool().getUser(userName).confirmSignUpInBackground(confirmCode, true, confHandler);
            }
        });

    }

    /*
* Function Name: GenericHandler
* Input: None
* Output: None
* Logic: It checks whether the user has entered the correct verification code or not
* Example Call: GenericHandler();
*
*/

    GenericHandler confHandler = new GenericHandler() {

        /*
* Function Name: onSuccess
* Input: None
* Output: None
* Logic: It calls a function createUser()
* Example Call: onSuccess();
*
*/

        @Override
        public void onSuccess() {


            createUser();

            Intent userActivity = new Intent(SignUpConfirm.this, HomeActivity.class);
            //userActivity.putExtra("name", username);
            startActivityForResult(userActivity, 4);

        }

        /*
* Function Name: onFailure
* Input: None
* Output: None
* Logic: None
* Example Call: onFailure(Exception);
*
*/

        @Override
        public void onFailure(Exception exception) {

        }
    };

    /*
* Function Name: createUser
* Input: None
* Output: None
* Logic: It inserts the username, emailid and current timestamp in insmartlock-mobilehub-1560124461-user_details table present in
* amazon dynamo db
* Example Call: createUser();
*
*/

    public void createUser() {
        final UserDetailsDO userdata = new UserDetailsDO();

        userdata.setUsername(userName);

        userdata.setEmailid(emailid);

        String currentDateTimeString = DateFormat.getDateTimeInstance().format(new Date());

        userdata.setTimeofcreation(currentDateTimeString);

        new Thread(new Runnable() {
            @Override
            public void run() {
                dynamoDBMapper.save(userdata);
                // Item saved
            }
        }).start();
    }

}
