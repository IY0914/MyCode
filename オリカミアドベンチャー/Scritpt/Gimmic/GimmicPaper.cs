using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class GimmicPaper : MonoBehaviour
{
    //�v���C���[�̃R���g���[���[
    GameObject player;
    //�e�L�X�g���b�V��
    Text text;

    void Start()
    {
        //�v���C���[�̏��擾
        player = GameObject.FindGameObjectWithTag("Player");
        //�e�L�X�g�̎擾
        text = GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        //�e�L�X�g�̕ϊ�
        text.text = "�~" + player.GetComponent<PlayerController>().paperNum;
    }
}
