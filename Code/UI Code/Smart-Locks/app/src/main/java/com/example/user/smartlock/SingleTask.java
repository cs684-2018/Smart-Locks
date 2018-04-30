

/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: SingleTask.java
* Theme: SmartLock
* Functions: onCreate(Bundle), GenericHandler(), onSuccess(), onFailure(Exception)
* onBindViewHolder(ViewHolder, int), getItemCount(), ViewHolder(View)
* Global Variables: username, dateofcreation, Username, TimeOfCreation, toolbar, dynamoDBClient,
* dynamoDBMapper, remove
*/

package com.example.user.smartlock;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUser;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.handlers.GenericHandler;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBMapper;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClient;


/**
 * Created by user on 16/4/18.
 */

public class SingleTask extends AppCompatActivity{


    private String username,dateofcreation;
    private TextView Username,TimeOfCreation;
    Toolbar toolbar;
    AmazonDynamoDBClient dynamoDBClient;
    private Button remove;
    DynamoDBMapper dynamoDBMapper;

        /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file
* and sets up a connection with AWS iot
* Example Call: onCreate(Bundle);
*
*/

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.singleuser);

        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);




        // Instantiate a AmazonDynamoDBMapperClient
        dynamoDBClient = new AmazonDynamoDBClient(AWSMobileClient.getInstance().getCredentialsProvider());
        this.dynamoDBMapper = DynamoDBMapper.builder()
                .dynamoDBClient(dynamoDBClient)
                .awsConfiguration(AWSMobileClient.getInstance().getConfiguration())
                .build();

        username = getIntent().getExtras().getString("Username");
        dateofcreation = getIntent().getExtras().getString("TimeofCreation");

        Log.d("msg1",username);
        Log.d("msg2",dateofcreation);

        Username = (TextView)findViewById(R.id.username);
        TimeOfCreation = (TextView)findViewById(R.id.timeofcreation);
        remove=(Button) findViewById(R.id.delete);

        Username.setText(username);
        TimeOfCreation.setText(dateofcreation);

        final CognitoUser user = AppHelper.getPool().getCurrentUser();

    }

          /*
* Function Name: GenericHandler
* Input: None
* Output: None
* Logic: It is used for deleting a user
* and sets up a connection with AWS iot
* Example Call: GenericHandler();
*
*/

    GenericHandler handler = new GenericHandler() {

             /*
* Function Name: onSuccess
* Input: None
* Output: None
* Logic: If a user is successfully deleted then this function takes the user to AllUsers Activity
* Example Call: onSuccess();
*
*/


        @Override
        public void onSuccess() {

           // deleteUser();

            Intent userActivity = new Intent(SingleTask.this, AllUsers.class);
            //userActivity.putExtra("name", username);
            startActivityForResult(userActivity, 4);

        }

        /*
* Function Name: onFailure
* Input: exception -> Exception
* Output: None
* Logic: It prints the exception message
* Example Call: onFailure(Exception);
*
*/

        public void onFailure(Exception exception) {
            Log.d("msg","exception");
            Log.d("msg",String.valueOf(exception.getMessage()));

        }
    };

    /*
* Function Name: deleteUser
* Input: None
* Output: None
* Logic: It deletes the user from smartlock-mobilehub-1560124461-user_details table present in dynamoDb
* Example Call: deleteUser();
*
*/

    public void deleteUser() {
        final UserDetailsDO userdata = new UserDetailsDO();

       // AmazonDynamoDBClient client = new AmazonDynamoDBClient(new ProfileCredentialsProvider());

       /* userdata.setUsername(userName);

        userdata.setEmailid("pratik.s.wagh95@gmail.com");

        String currentDateTimeString = DateFormat.getDateTimeInstance().format(new Date());

        userdata.setTimeofcreation(currentDateTimeString);*/

        /*new Thread(new Runnable() {
            @Override
            public void run() {

                DynamoDB dynamoDB = new DynamoDB(new AmazonDynamoDBClient(
                        new ProfileCredentialsProvider()));

                AmazonDynamoDB client = AmazonDynamoDBClientBuilder.standard()
                        .withEndpointConfiguration(new AwsClientBuilder.EndpointConfiguration("http://localhost:8000", "us-west-2"))
                        .build();

                DynamoDB dynamoDB = new DynamoDB(client);

                Table table = dynamoDB.getTable("Movies");

                AmazonDynamoDB client = AmazonDynamoDBAsyncClientBuilder.standard().build();

                DynamoDB dynamoDB = new DynamoDB(client);

                Table table = dynamoDB.getTable("userdetails");

                CognitoUser user = AppHelper.getPool().getCurrentUser();

                DeleteItemOutcome outcome = table.deleteItem("username",user.getUserId());
               // dynamoDBMapper.save(userdata);
                // Item saved
            }
        }).start();*/
    }

}
