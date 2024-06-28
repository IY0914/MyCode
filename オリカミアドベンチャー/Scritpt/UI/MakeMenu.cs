using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

public class MakeMenu : MonoBehaviour
{
    //�܂莆���j���[�̃L�����o�X
    public GameObject menuCanvas;
    //�܂莆��ʂ̃L�����o�X
    public GameObject origamiCanvas;
    //�|�[�Y�p�l��
    public GameObject posePanel;
    //�ړ��O�ʒu
    public Vector2 beforePos;
    //�ړ���ʒu
    public Vector2 afterPos;

    //UI���x
    private const float UISpeed = 5.0f;
    //����
    private float time = 0.0f;
    //�ړ��ϐ�
    private bool moveflag = false;


    // Start is called before the first frame update
    void Start()
    {
        //�n�܂������\��
        origamiCanvas.SetActive(false);
        posePanel.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        //���ԂƑ��x���|����
        if(moveflag) time += Time.unscaledDeltaTime * UISpeed;
        else time -= Time.unscaledDeltaTime * UISpeed;

        if(time > 1) time = 1;
        else if(time < 0) time = 0;

        if (Input.GetKeyDown(KeyCode.Escape) && !moveflag)
        {
            //�t���O�Ǘ�
            moveflag = true;
           
        }
        else if(Input.GetKeyDown(KeyCode.Escape) && moveflag)
        {
            //�t���O�Ǘ�
            moveflag = false;
        }

        if (time == 1 && moveflag)
        {
            posePanel.SetActive(true); //�|�[�Y�̃p�l����\��
            Time.timeScale = 0; //�������~�߂�
        }
        else
        {
            posePanel.SetActive(false); //�|�[�Y�̃p�l����\��
            Time.timeScale = 1; //�������~�߂�
        }

            //���j���[���J��
            menuCanvas.transform.position = Vector2.Lerp(afterPos, beforePos, time);
    }

    public void OnClickClose()
    {
        //��������悤�ɂ���
        Time.timeScale = 1;

        posePanel.SetActive(false); //�|�[�Y�̃p�l�����\��

        //�����̊֌W
        if (moveflag) moveflag = false;
        else moveflag = true;
            
    }

    public void ExitButton()
    {
        //�I������
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("StageSelect");
        
    }
}
