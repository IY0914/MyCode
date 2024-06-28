using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AttackCoolUI : MonoBehaviour
{
    //�C���[�W�̃R���|�[�l���g
    private Image image;
    //�R���g���[���[
    private PlayerController playerController;
    //�t���[���̈ړ��ɍ��킹�Ȃ����߂Ɏ擾
    public GameObject frame;
    
    // Start is called before the first frame update
    void Start()
    {
        //�擾
        image = GetComponent<Image>();
        //�h��Ԃ���ݒ�
        image.fillAmount = 0;
        //�^�O�Ŏ擾
        playerController = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //�N�[���^�C����UI���쐬
        image.fillAmount = playerController.specialCool / 3.0f;

        //�O�ȉ��̏ꍇ
        if(image.fillAmount <= 0)
        {
            this.transform.position = frame.transform.position;
        }
    }
}
