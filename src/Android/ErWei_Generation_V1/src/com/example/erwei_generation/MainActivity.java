package com.example.erwei_generation;

import java.util.Hashtable;



import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;


public class MainActivity extends Activity {

    Button creater_qr;
    ImageView iv;
    EditText et_qr;
    Activity act;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        act = this;
        initView() ;
    }


    private void initView() {

        et_qr = (EditText) findViewById(R.id.et_qr);
        creater_qr = (Button) findViewById(R.id.creater_qr);
        iv = (ImageView) findViewById(R.id.iv);

        creater_qr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String str = et_qr.getText().toString().trim();
                if (str.isEmpty()) {
                    Toast.makeText(MainActivity.this, "娌℃湁杈撳叆瀛楃涓�", Toast.LENGTH_SHORT).show();
                } else {
                    final Bitmap qrCodeBitmap = createQRCodeBitmap(str);

                    act.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            //灏嗗浘鏍囩殑bitmap杞寲鎴怋itmapDrawable
                            BitmapDrawable iconDrawable = new BitmapDrawable(qrCodeBitmap);
                            iv.setBackground(iconDrawable);
                        }
                    });
                }

            }
        });

    }


    /**
     * 鍒涘缓QR浜岀淮鐮佸浘鐗囨柟娉�
     */
    private Bitmap createQRCodeBitmap(String url) {
        // 鐢ㄤ簬璁剧疆QR浜岀淮鐮佸弬鏁�
        Hashtable<EncodeHintType, Object> qrParam = new Hashtable<EncodeHintType, Object>();
        // 璁剧疆QR浜岀淮鐮佺殑绾犻敊绾у埆鈥斺�旇繖閲岄�夋嫨鏈�楂楬绾у埆
        qrParam.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
        // 璁剧疆缂栫爜鏂瑰紡
        qrParam.put(EncodeHintType.CHARACTER_SET, "UTF-8");

        // 璁惧畾浜岀淮鐮侀噷闈㈢殑鍐呭锛岃繖閲屾垜閲囩敤鎴戝崥瀹㈢殑鍦板潃
        //   String cont = "http://blog.csdn.net/fengltxx";

        // 鐢熸垚QR浜岀淮鐮佹暟鎹�斺�旇繖閲屽彧鏄緱鍒颁竴涓敱true鍜宖alse缁勬垚鐨勬暟缁�
        // 鍙傛暟椤哄簭鍒嗗埆涓猴細缂栫爜鍐呭url鍦板潃锛岀紪鐮佺被鍨嬶紝鐢熸垚鍥剧墖瀹藉害锛岀敓鎴愬浘鐗囬珮搴︼紝璁剧疆鍙傛暟
        try {
            BitMatrix bitMatrix = new MultiFormatWriter().encode(url,
                    BarcodeFormat.QR_CODE, 180, 180, qrParam);

            // 寮�濮嬪埄鐢ㄤ簩缁寸爜鏁版嵁鍒涘缓Bitmap鍥剧墖锛屽垎鍒涓洪粦鐧戒袱鑹�
            int w = bitMatrix.getWidth();
            int h = bitMatrix.getHeight();
            int[] data = new int[w * h];

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    if (bitMatrix.get(x, y))
                        data[y * w + x] = 0xff000000;// 榛戣壊
                    else
                        data[y * w + x] = -1;// -1 鐩稿綋浜�0xffffffff 鐧借壊
                }
            }

            // 鍒涘缓涓�寮燽itmap鍥剧墖锛岄噰鐢ㄦ渶楂樼殑鏁堟灉鏄剧ず
            Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
            // 灏嗕笂闈㈢殑浜岀淮鐮侀鑹叉暟缁勪紶鍏ワ紝鐢熸垚鍥剧墖棰滆壊
            bitmap.setPixels(data, 0, w, 0, 0, w, h);
            return bitmap;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

}