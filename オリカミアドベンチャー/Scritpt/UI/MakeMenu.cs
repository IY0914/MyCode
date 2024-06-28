using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

public class MakeMenu : MonoBehaviour
{
    //折り紙メニューのキャンバス
    public GameObject menuCanvas;
    //折り紙画面のキャンバス
    public GameObject origamiCanvas;
    //ポーズパネル
    public GameObject posePanel;
    //移動前位置
    public Vector2 beforePos;
    //移動先位置
    public Vector2 afterPos;

    //UI速度
    private const float UISpeed = 5.0f;
    //時間
    private float time = 0.0f;
    //移動変数
    private bool moveflag = false;


    // Start is called before the first frame update
    void Start()
    {
        //始まったら非表示
        origamiCanvas.SetActive(false);
        posePanel.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        //時間と速度を掛ける
        if(moveflag) time += Time.unscaledDeltaTime * UISpeed;
        else time -= Time.unscaledDeltaTime * UISpeed;

        if(time > 1) time = 1;
        else if(time < 0) time = 0;

        if (Input.GetKeyDown(KeyCode.Escape) && !moveflag)
        {
            //フラグ管理
            moveflag = true;
           
        }
        else if(Input.GetKeyDown(KeyCode.Escape) && moveflag)
        {
            //フラグ管理
            moveflag = false;
        }

        if (time == 1 && moveflag)
        {
            posePanel.SetActive(true); //ポーズのパネルを表示
            Time.timeScale = 0; //処理を止める
        }
        else
        {
            posePanel.SetActive(false); //ポーズのパネルを表示
            Time.timeScale = 1; //処理を止める
        }

            //メニューを開く
            menuCanvas.transform.position = Vector2.Lerp(afterPos, beforePos, time);
    }

    public void OnClickClose()
    {
        //処理するようにする
        Time.timeScale = 1;

        posePanel.SetActive(false); //ポーズのパネルを非表示

        //処理の関係
        if (moveflag) moveflag = false;
        else moveflag = true;
            
    }

    public void ExitButton()
    {
        //終了処理
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("StageSelect");
        
    }
}
