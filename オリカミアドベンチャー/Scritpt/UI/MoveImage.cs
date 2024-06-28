using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class MoveImage : MonoBehaviour
{
    //�ʒu�̕ϐ�
    private Vector3 nowPos;
    public GameObject movePos;
    //��Ԋ֐���ON
    private bool lerpFlag = false;

    //�⊮����
    private float time;

    // Start is called before the first frame update
    void Start()
    {
        //�ŏ��̈ʒu��ݒ�
        nowPos = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        //�ړ��t���O�ňړ�
        if(lerpFlag)
        {
            time += Time.deltaTime;
        }
        else if(!lerpFlag && time > 0.0f)
        {
            time -= Time.deltaTime;
        }

        //1�ȏ�̎��O�ȉ��̎�
        if (time > 1) time = 1;
        else if (time < 0) time = 0;

        transform.position = Vector3.Lerp(nowPos, movePos.transform.position, time);
    }

    //�t���O�̊Ǘ�
    public void ChangedFlags()
    {
        if (lerpFlag) lerpFlag = false;
        else lerpFlag = true;
    }
}
