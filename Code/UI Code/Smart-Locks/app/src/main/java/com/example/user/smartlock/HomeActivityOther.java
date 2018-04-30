
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh, Rahul Sharma
* Filename: HomeActivityOther.java
* Theme: SmartLock
* Functions: onCreate(Bundle), insertintolog(), onOptionsItemSelected()
* Global Variables: Drawer, Toggle, toolbar, unlock, dynamoDBMapper, CUSTOMER_SPECIFIC_ENDPOINT,
*                   COGNITO_POOL_ID, MY_REGION, mqttManager, clientId, flag, credentialsProvider
*
*/

package com.example.user.smartlock;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.NavigationView;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import java.io.UnsupportedEncodingException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;

import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobileconnectors.cognitoidentityprovider.CognitoUser;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBMapper;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClient;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by user on 4/4/18.
 */

public class HomeActivityOther extends AppCompatActivity {


    private DrawerLayout Drawer;
    private ActionBarDrawerToggle Toggle;
    Toolbar toolbar;
    static int flag;

    Button unlock;

    // Customer specific IoT endpoint
    // AWS Iot CLI describe-endpoint call returns: XXXXXXXXXX.iot.<region>.amazonaws.com,
    private static final String CUSTOMER_SPECIFIC_ENDPOINT = "a1kw7adkqhyy7v.iot.us-east-2.amazonaws.com";

    // Cognito pool ID. For this app, pool needs to be unauthenticated pool with
    // AWS IoT permissions.
    private static final String COGNITO_POOL_ID = "us-east-2:cbd064f2-8dfd-4bf6-89a6-341e4d22d699";

    // Region of AWS IoT
    private static final Regions MY_REGION = Regions.US_EAST_2;

    AWSIotMqttManager mqttManager;
    String clientId;

    CognitoCachingCredentialsProvider credentialsProvider;
    DynamoDBMapper dynamoDBMapper;

      /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file,
* It sets up a connection with AWS Iot for subsribing and publishing the data to shadow link
* It also creates a logic for the side drawer, which means if any of the option selection from the drawer
* what action has to be performed
* Example Call: onCreate(Bundle);
*
*/

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_other);

        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar actionbar = getSupportActionBar();
        actionbar.setDisplayHomeAsUpEnabled(true);
        actionbar.setHomeAsUpIndicator(R.drawable.ic_menu);
        // Instantiate a AmazonDynamoDBMapperClient
        AmazonDynamoDBClient dynamoDBClient = new AmazonDynamoDBClient(AWSMobileClient.getInstance().getCredentialsProvider());
        this.dynamoDBMapper = DynamoDBMapper.builder()
                .dynamoDBClient(dynamoDBClient)
                .awsConfiguration(AWSMobileClient.getInstance().getConfiguration())
                .build();


        unlock=(Button) findViewById(R.id.unlock);

        Drawer=(DrawerLayout) findViewById(R.id.drawerlayout);
        NavigationView navigationView=findViewById(R.id.nav_view);



        navigationView.setNavigationItemSelectedListener(
                new NavigationView.OnNavigationItemSelectedListener() {
                    @Override
                    public boolean onNavigationItemSelected(MenuItem menuItem) {
                        int id = menuItem.getItemId();
                        switch (id) {

                            case R.id.generatepattern:
                                Intent d = new Intent(HomeActivityOther.this, GeneratePattern.class);
                                startActivity(d);
                                break;


                            case R.id.changepassword:
                                Intent f = new Intent(HomeActivityOther.this, ChangePasswordOther.class);
                                startActivity(f);
                                break;

                            case R.id.logout:
                                Intent g = new Intent(HomeActivityOther.this, Logout.class);
                                startActivity(g);
                                break;
                        }

                        return true;
                    }
                });

        Toggle=new ActionBarDrawerToggle(this,Drawer,R.string.open,R.string.close);

        Drawer.addDrawerListener(Toggle);
        Toggle.syncState();

        getSupportActionBar().setDisplayShowHomeEnabled(true);

        clientId = UUID.randomUUID().toString();


        // Initialize the AWS Cognito credentials provider
        credentialsProvider = new CognitoCachingCredentialsProvider(
                getApplicationContext(), // context
                COGNITO_POOL_ID, // Identity Pool ID
                MY_REGION // Region
        );

        // MQTT Client
        mqttManager = new AWSIotMqttManager(clientId, CUSTOMER_SPECIFIC_ENDPOINT);



//unlocking the lock
        try {

            Log.d("msg", "Status = ");
            mqttManager.connect(credentialsProvider, new AWSIotMqttClientStatusCallback() {
                @Override
                public void onStatusChanged(final AWSIotMqttClientStatus status,
                                            final Throwable throwable) {
                    Log.d("Status", "Status = " + String.valueOf(status));

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if (status == AWSIotMqttClientStatus.Connecting) {

                                Log.d("Status","Connecting..." );

                            }
                            else if (status == AWSIotMqttClientStatus.Connected) {

                                Log.d("Status","Connected" );

                                try {

                                    final String topic = "$aws/things/PubSub/shadow/update";

                                    Log.d("msg", "2");

                                    Log.d("topic", "topic = " + topic);

                                    mqttManager.subscribeToTopic(topic, AWSIotMqttQos.QOS0,
                                            new AWSIotMqttNewMessageCallback() {

                                                @Override
                                                public void onMessageArrived(final String topic, final byte[] data) {
                                                    Log.d("message", "inside");
                                                    runOnUiThread(new Runnable() {
                                                        @Override
                                                        public void run() {
                                                            try {
                                                                String message = new String(data, "UTF-8");

                                                                Log.d("message", message);


                                                                try {
                                                                    JSONObject reader = new JSONObject(message);
                                                                    JSONObject sys  = reader.getJSONObject("state");
                                                                    JSONObject sys1  = sys.getJSONObject("desired");
                                                                    String status = sys1.getString("message");
                                                                    Log.d("message", status);
                                                                    if(status == "unlock"){

                                                                        flag=1;
                                                                    }
                                                                } catch (JSONException e) {
                                                                    e.printStackTrace();
                                                                }

                                                                String hi="abcd";






                                                                Log.d("msg", "1");

                                                            } catch (UnsupportedEncodingException e) {
                                                                Log.d("error", "Message encoding error.", e);
                                                            }
                                                        }
                                                    });
                                                }
                                            });
                                } catch (Exception e) {
                                    Log.e("error", "Subscription error.", e);
                                }




                            }
                            else if (status == AWSIotMqttClientStatus.Reconnecting) {
                                if (throwable != null) {
                                    Log.d("Status", "Connection error.", throwable);
                                }

                            }
                            else if (status == AWSIotMqttClientStatus.ConnectionLost) {
                                if (throwable != null) {
                                    Log.d("Status", "Connection error.", throwable);
                                    throwable.printStackTrace();
                                }

                            }
                        }
                    });
                }
            });
        } catch (final Exception e) {
            Log.e("error", "Connection error." + e.getMessage());

        }



        if(flag==1)
            unlock.setEnabled(false);


        unlock.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {


                final String topic = "$aws/things/PubSub/shadow/update";
                final String msg = "{\"state\": {\"desired\": {\"message\": \"unlock\" }}}";

                try {
                    mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);

                    insertintolog();



                } catch (Exception e) {
                    Log.d("Status", "Publish error.", e);
                }

            }
        });


    }

    /*
* Function Name: insertintolog
* Input: None
* Output: None
* Logic: It insert the username and current timestamp into smartlock-mobilehub-1560124461-logs
* table present in amazon dynamo db
* Example Call: insertintolog();
*
*/

    public void insertintolog() {
        final LogsDO logdata = new LogsDO();
        CognitoUser user = AppHelper.getPool().getCurrentUser();

        Log.d("msg", user.toString());

        String Username = user.getUserId();

        logdata.setUserId(Username);


        DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Date date = new Date();
        // System.out.println(dateFormat.format(date));

        String currentDateTimeString = dateFormat.format(date);



        logdata.setTimestamp(currentDateTimeString);

        new Thread(new Runnable() {
            @Override
            public void run() {
                dynamoDBMapper.save(logdata);

                // Item saved
            }
        }).start();
    }

    /*
 * Function Name: onOptionsItemSelected
 * Input: None
 * Output: None
 * Logic: It checks which option is selected by a user from the side drawer and accordingly performs an operation
 * Example Call: onOptionsItemSelected(MenuItem);
 *
 */

    @Override
    public  boolean onOptionsItemSelected(MenuItem item){

        if(Toggle.onOptionsItemSelected(item)){
            return true;

        }
        return super.onOptionsItemSelected(item);
    }


}
