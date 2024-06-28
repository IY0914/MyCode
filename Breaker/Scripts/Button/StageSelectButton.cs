using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageSelectButton : MonoBehaviour
{
    public GameObject ButtonPanel;

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            Time.timeScale = 0;
            this.ButtonPanel.SetActive(true);
        }
    }

    public void YesButton()
    {
        Time.timeScale = 1;
        SceneManager.LoadScene("Stage1Scene");
    }
    public void NoButton()
    {
        Time.timeScale = 1;
        this.ButtonPanel.SetActive(false);
    }
}
