using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoulEffect : MonoBehaviour
{
    //�p�[�e�B�N�����擾
    public ParticleSystem particleSystem;
    public GameObject resultPanel;

    private void OnTriggerEnter(Collider other)
    {

        if(other.gameObject.tag == "Player")
        {
            //�Đ�
            particleSystem.Play();
            //�S�[���I�u�W�F�N�g�̍폜
            Destroy(this.gameObject);

            //�v���C���[�̃R���g���[����؂�
            other.gameObject.GetComponent<PlayerController>().Controlller = false;

            resultPanel.SetActive(true);
        }
    }
}
