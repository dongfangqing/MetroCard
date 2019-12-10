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
        AlertDialog alertDialog = new AlertDialog.Builder(ScannerActivity.this).setTitle("�� ��")
                .setMessage(url).setNegativeButton("ȡ��", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        finish();
                    }

                }).setPositiveButton("ȷ��", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
//                        if (url.substring(0, 4).equals("http")) {
//                            OpenURL(url);
//                        } else {
//                        }
//                    	String menoy_p  = sp.getString("name", "");
                    	//Ҫ������ַ���
                    	//Toast.makeText(ScannerActivity.this, url, Toast.LENGTH_LONG).show();
                    	
                    	 ss.Send_To_Serv( "#" , url);//���ͺ���	
//                        Intent inten = new Intent(ScannerActivity.this, MainActivity.class);
//                        inten.putExtra("RMB" ,"hello");    //�������ݵ�������
//                        startActivity(inten);
                    	 
                         Intent intent=new Intent();//����������ͼ����  
                         
                         intent.setClass(ScannerActivity.this, MainActivity.class);  
                         //setClass,��һ������packageContext��Activity��Context�����࣬���Կ�������ת��  
                         //��֮��һ����������activity�������  
                           
                         //�ڶ�������cls����Ҫ������һ��activity  
                           
 //                        intent.putExtra("RMB", "Dick");  
                         //�Ž����ݡ���һ��������Ϊ�����İ���+������  
                           
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
