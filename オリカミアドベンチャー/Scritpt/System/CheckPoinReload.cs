using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckPoinReload : MonoBehaviour
{
    //�ʉ߂��čX�V������
    public bool check = false;

    //�X�e�[�g�I�u�W�F�N�g
    private PlayerState playerState;

    private void Start()
    {
        playerState = GameObject.Find("PlayerStateCot").GetComponent<PlayerState>();
    }

    private void OnTriggerEnter(Collider other)
    {
        //��x�ʂ����ꍇ�͏������Ȃ�
        if (check) return;

        //�v���C���[�̍X�V
        if (other.gameObject.tag == "Player")
        {
            //���̕�[
            other.gameObject.GetComponent<PlayerController>().paperNum += 10;
            //�X�e�[�g�̃`�F�b�N�|�C���g�̐؂�ւ�
            playerState.checkPoint.position = this.transform.position;

            //�����؂�ւ���
            check = true;
        }
    }
}
