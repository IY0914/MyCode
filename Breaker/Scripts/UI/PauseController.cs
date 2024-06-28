using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class PauseController : MonoBehaviour
{
    public GameObject Panel;

    //ポーズ画面にする
    public void PauseGame()
    {
        Time.timeScale = 0;
        this.Panel.SetActive(true);
    }

    //ポーズ画面解除
    public void ReStartGame()
    {
        Time.timeScale = 1;
        this.Panel.SetActive(false);
    }

    //ステージセレクト画面に戻る
    public void BackStage()
    {
        Time.timeScale = 1;
        SceneManager.LoadScene("StageSelectScene");
    }

    //タイトル画面に戻る
    public void BackTitle()
    {
        Time.timeScale = 1;
        SceneManager.LoadScene("TitleScene");
    }
}