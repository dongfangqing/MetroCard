package com.example.mealcard;


import com.example.socket.MySocket;
import com.example.svner.ScannerActivity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity implements OnClickListener{

    private Button bt_scan;
	private TextView tv_money;
	private Button bt_xj;
	private MySocket ss;
	private Handler handler;
	private String recv_Mag;
    private SharedPreferences sp;
	private Editor editor;
	private String rMB_Value ;
	private String menoy_p = null;
	private boolean flag = true;
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        sp = getSharedPreferences("mysp", MODE_PRIVATE);
        editor = sp.edit();
        
        
     
        ss = new MySocket();             //����������
        Intent intent = getIntent();        //��ʼ��ͼ
        String rMB_Value = intent.getStringExtra("RMB"); 
//        
//        Toast.makeText(MainActivity.this, rMB_Value  + "222", Toast.LENGTH_SHORT).show();
        
//        rMB_Value  = it.getStringExtra("RMB");
        if(rMB_Value == null || rMB_Value.length() <= 0)
        {
        	
        }else
        {
 //         tv_money.setText(rMB_Value );
   //       Toast.makeText(MainActivity.this, rMB_Value  + "222", Toast.LENGTH_SHORT).show();
        }
  
        //������ȡ���ݵ��߳�
        new ReadThread().start();  
 

       
        bt_scan = (Button)findViewById(R.id.bt_Scan);         //ɨ�谴��
        bt_xj = (Button)findViewById(R.id.bt_js);           //ѧУ���ܰ���
        
        
        tv_money = (TextView)findViewById(R.id.tv_q);       //����TextView
               

        bt_xj.setOnClickListener(this);

        //����ɨ�븶��
        bt_scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //��ת��ɨ�븶��
                Intent inten = new Intent(MainActivity.this, ScannerActivity.class);
                startActivity(inten);

            }
        });     
        

        menoy_p  = sp.getString("name", "");
        if(menoy_p == null || menoy_p.length() <= 0)
        {
        	
        }else
        {
          tv_money.setText(menoy_p );
//          Toast.makeText(MainActivity.this, menoy_p  + "RMB", Toast.LENGTH_SHORT).show();
        }

      
        //��ȡ���ݸ��½���
        handler = new Handler(){

			private int ret, i;
			private String name          ;


			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				
				//�������̷߳�����Ϣ����,�ͻ��Զ�ִ��
				if(msg.what ==2){
					
					Bundle bundle = msg.getData();
					recv_Mag = bundle.getString("Recv_Mag").toString();
		
	//				Log.i("shen","recv_Mag:"+recv_Mag);
				   
					editor.putString("name", recv_Mag);
					boolean isSuccess = editor.commit();//�ύ

	//				Toast.makeText(MainActivity.this, recv_Mag  +"RMB", 1000).show();
			
					tv_money.setText(recv_Mag);
				
			  }
			}
        };
 
    }


	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		
		Toast.makeText(MainActivity.this, "У԰����", 1000).show();
        //��ת��ɨ�븶��
        Intent inten = new Intent(MainActivity.this, SchoolActivity.class);
        startActivity(inten);
		
	}
	
	//��ȡ�߳�+handler
	class ReadThread extends Thread{
		
		private String str;

		@Override
		public void run() {
			// TODO Auto-generated method stub
			
	        while(flag)
	        {
        	
	        	 //��ʱ����
	        	 try {
					ReadThread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	        	
	        	 str = ss.Recv_From_Serv();  

	        	 Log.e("Recv_From_Serv", str);
	        	
//	    		Message msg = new Message();
	    		Message msg = Message.obtain();//���ڴ�����Message����,���Է���,û�о�new Message(),�ɽ�ʡ��Դ
	    		msg.what = 2;//��ʶ��
	        	
	        	Bundle bunlde = new Bundle();
	    		bunlde.putString("Recv_Mag", str);
	    		msg.setData(bunlde);
	    		handler.sendMessage(msg);
	        }
		}
	}
	
	
	
}
