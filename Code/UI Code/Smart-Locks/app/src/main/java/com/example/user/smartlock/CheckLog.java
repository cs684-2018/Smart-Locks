
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh, Rahul Sharma
* Filename: CheckLog.java
* Theme: SmartLock
* Functions: onCreate(Bundle), logs()
* Global Variables: toolbar, dynamoDBMapper, dynamoDBClient, recyclerView, logAdapter, result
*/


package com.example.user.smartlock;

import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBScanExpression;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBMapper;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClient;
import java.util.ArrayList;
import java.util.List;


/**
 * Created by user on 19/3/18.
 */

public class CheckLog extends AppCompatActivity {


    Toolbar toolbar;
    DynamoDBMapper dynamoDBMapper;
    AmazonDynamoDBClient dynamoDBClient;
    private RecyclerView recyclerView;
    private LogAdapter logAdapter;
    private List<LogsDO> result = new ArrayList<>();

        /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file,
* It sets up a connection with AWS Iot and calls the function logs()
* Example Call: onCreate(Bundle);
*
*/

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alllog);

        recyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        RecyclerView.LayoutManager mLayoutManager = new LinearLayoutManager(getApplicationContext());
        recyclerView.setLayoutManager(mLayoutManager);
        recyclerView.setHasFixedSize(true);


        // Instantiate a AmazonDynamoDBMapperClient
        dynamoDBClient = new AmazonDynamoDBClient(AWSMobileClient.getInstance().getCredentialsProvider());
        this.dynamoDBMapper = DynamoDBMapper.builder()
                .dynamoDBClient(dynamoDBClient)
                .awsConfiguration(AWSMobileClient.getInstance().getConfiguration())
                .build();


        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        try {
            logs();
        } catch (Exception e) {
            e.printStackTrace();
        }
        SystemClock.sleep(3000);
        logAdapter=new LogAdapter(this,result);
        recyclerView.setAdapter(logAdapter);

        Log.d("msg","reached");
    }

           /*
* Function Name: logs
* Input: None
* Output: None
* Logic: It scans the smartlock-mobilehub-1560124461-logs table present in aws and prints each entry in it.
* Example Call: logs();
*
*/

    public void logs() throws Exception {


        new Thread(new Runnable() {
            @Override
            public void run() {

                DynamoDBMapper mapper = new DynamoDBMapper(dynamoDBClient);


                System.out.println("Scanning Tesis");


                DynamoDBScanExpression scanExpression = new DynamoDBScanExpression();

                System.out.println("Scanning Tesis");


                result = mapper.scan(LogsDO.class, scanExpression);
                System.out.println("Scanning Tesis");
                System.out.println(result.toString());
                Log.d("msg",result.toString());

                for (LogsDO log : result) {
                    Log.d("user", String.valueOf(log));
                    System.out.println(log.getUserId());

                    System.out.println(log.getTimestamp());

                }

            }
        }).start();

    }
}
