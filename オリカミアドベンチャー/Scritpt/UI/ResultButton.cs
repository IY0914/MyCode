using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ResultButton : MonoBehaviour
{
   public void ReturnStageSelect()
    {
        SceneManager.LoadScene("StageSelect");
    }

    public void ReturnTitle()
    {
        SceneManager.LoadScene("TitleScene");
    }
    public void Restart()
    {
        SceneManager.LoadScene("Stage1");
    }

    public void End()
    {
        Application.Quit();
    }
}
