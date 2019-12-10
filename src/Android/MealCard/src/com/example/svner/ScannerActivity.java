package  com.example.svner;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Toast;

import com.covics.zxingscanner.OnDecodeCompletionListener;
import com.covics.zxingscanner.ScannerView;
import com.example.mealcard.IPSetActivity;
import com.example.mealcard.MainActivity;
import com.example.mealcard.R;
import com.example.socket.MySocket;

public class ScannerActivity extends Activity implements OnDecodeCompletionListener {

    private ScannerView scannerView;
    private String string;
	private MySocket ss;
    private SharedPreferences sp;
	private Editor editor;
	
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        sp = getSharedPreferences("mysp", MODE_PRIVATE);
        editor = sp.edit();
        
        ss = new MySocket();

        setContentView(R.layout.activity_scan);
        init();
    }

    private void init() {
        scannerView = (ScannerView) findViewById(R.id.scanner_view);
        scannerView.setOnDecodeListener(this);
    }


    @Override
    public void onDecodeCompletion(String barcodeFormat, String barcode, Bitmap bitmap) {
        if (barcode == null || "".equals(barcode)) {

            AlertDialog builder = new AlertDialog.Builder(ScannerActivity.this).
                    setTitle("Error").setMessage("Not Found").show();
        } else {
            string = barcode.substring(barcode.indexOf("?") + 1, barcode.length());
            mShowDialog(string);
        }

    }


    @Override
    protected void onResume() {
        super.onResume();
        scannerView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        scannerView.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    /**/

    private void mShowDialog(final String url) {
        AlertDialog alertDialog = new AlertDialog.Builder(ScannerActivity.this).setTitle("金 额")
                .setMessage(url).setNegativeButton("取消", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        finish();
                    }

                }).setPositiveButton("确定", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
//                        if (url.substring(0, 4).equals("http")) {
//                            OpenURL(url);
//                        } else {
//                        }
//                    	String menoy_p  = sp.getString("name", "");
                    	//要传输的字符串
                    	//Toast.makeText(ScannerActivity.this, url, Toast.LENGTH_LONG).show();
                    	
                    	 ss.Send_To_Serv( "#" , url);//发送函数	
//                        Intent inten = new Intent(ScannerActivity.this, MainActivity.class);
//                        inten.putExtra("RMB" ,"hello");    //传输数据到主界面
//                        startActivity(inten);
                    	 
                         Intent intent=new Intent();//首先生成意图对象  
                         
                         intent.setClass(ScannerActivity.this, MainActivity.class);  
                         //setClass,第一个参数packageContext，Activity是Context的子类，所以可以向上转型  
                         //总之第一个参数，传activity对象进来  
                           
                         //第二个参数cls。你要启动哪一个activity  
                           
 //                        intent.putExtra("RMB", "Dick");  
                         //放进数据。第一个参数，为完整的包名+变量名  
                           
                         startActivity(intent); 
                    	
                    	 finish();
                    }

                }).show();
    }

    
    private void OpenURL (String url) {
        Uri uri = Uri.parse(url);
        Intent intent = new Intent(Intent.ACTION_VIEW, uri);
        startActivity(intent);
        finish();
    }


}
