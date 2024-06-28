using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class ButtonClick : MonoBehaviour
{
    //押したかどうかの判定取り
    bool buttonClick = false;
    //時間
    float Timer = 0.0f;
    public float FadeTime = 0.0f;
    //フェイドアウトに必要なキャンバス
    public GameObject canvas;

    public void PlayClick()
    {
        buttonClick = true;
        canvas.SetActive(true);
    }

    public void RetryClick()
    {
        SceneManager.LoadScene("Stage1Scene");
    }
    public void NextStage()
    {
        SceneManager.LoadScene("Stage2Stage");
    }

    public void StageSelectClick()
    {
        SceneManager.LoadScene("StageSelectScene");
    }

    public void ExitClick()
    {
        Application.Quit();
    }

    private void Update()
    {
        if(buttonClick)
        {
            Timer += Time.deltaTime;
            Debug.Log(Timer);
        }

        if(Timer >= FadeTime)
        {
            SceneManager.LoadScene("StageSelectScene");
        }
    }
}
