using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class EnemyLife : MonoBehaviour
{
    //�̗�
    public int Life = 3;
    //�X���C�_�[
    public Slider slider;
    //�ʒu���
    private Vector3 pos;

    //�v���C���[���Q��
    PlayerController playerController;
    //�G�t�F�N�g
    ParticleSystem particle;

    // Start is called before the first frame update
    void Start()
    {
        //�X���C�_�[�̍ő�l��ς���
        slider.maxValue = Life;
        //�X���C�_�[���}�b�N�X�ɂ���
        slider.value = Life;
        //�擾
        playerController = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
        //�p�[�e�B�N���̎擾
        particle = this.GetComponent<ParticleSystem>();
    }

    // Update is called once per frame
    void Update()
    {
        //�X���C�_�[�̑���
        slider.value = Life;
        //HP�������Ȃ��������
        if (Life <= 0) Destroy(this.gameObject);
    }

    private void OnCollisionEnter(Collision collision)
    {
        //�U�����Ƃɑ̗͂����炷
        if (collision.gameObject.tag == "Gimmic") 
        {
            Life -= 3;

            //�ڐG����̈ʒu
            pos = collision.contacts[0].point;
        }
    }

}
