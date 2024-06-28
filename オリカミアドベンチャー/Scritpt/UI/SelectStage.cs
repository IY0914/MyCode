using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SelectStage : MonoBehaviour
{
    //�X�e�[�W�̃��X�g
    public List<GameObject> stageObject;
    //�X�e�[�W�����X�g
    public List<string> scenes = new List<string>();
    //�I���X�e�[�W�ԍ�
    public int  stageNumber = 0;
    //�L�����o�X�̃R���[�`���𓮂���
    public TitleButtonFlag flag;

    private void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        //����ł��邩���擾
        for(int i = 0; i < stageObject.Count; i++)
        {
            if (stageObject[i].GetComponent<StagePush>().playerpush)
            {
                //����ł����ꍇ�ԍ����擾
                stageNumber = i;
            }
        }

        if (Input.GetKeyDown(KeyCode.Return))
        {
            flag.Botton();

            SceneManager.LoadScene(scenes[stageNumber]);
        }
    }

    void StageOpen()
    {
        
    }
}
