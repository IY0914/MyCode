using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerState : MonoBehaviour
{
    //��Ԃ�F�ŕ\��(��)
    Material material;
    //�v���C���[�R���g���[���[
    public PlayerController controller;
    //�v���C���[�I�u�W�F�N�g
    public GameObject Player;

    //�D���
    public GameObject ShipObject;
    //�O���C�_�[���
    public GameObject GliderObject;
    //���b�V��
    public GameObject MeshObject;

    //��Ԃ̑J�ڂ��擾
    public bool humanState = false;
    public bool gliderState = false;
    public bool shipState = false;

    //�`�F�b�N�|�C���g
    public Transform checkPoint;

    //�ʒu�̍X�V��bool
    private bool change = false;

    // Start is called before the first frame update
    void Start()
    {
        //�擾
        material = this.gameObject.GetComponent<Material>();
    }

// Update is called once per frame
void Update()
    {
        //�e��Ԃ��擾
        gliderState = controller.glider;
        shipState = controller.ship;

        //���ȏ㗎����ƃ`�F�b�N�|�C���g�ɖ߂�
        if(Player.transform.position.y < -5.0f)
        {
            Player.transform.position = checkPoint.position;
            //�̗͂����
            Player.GetComponent<PlayerController>().HP -= 1;

        }

        if(controller.invisibleTime != 0.0f) MeshObject.SetActive(false);

        //��Ԃ̑J�ڂƌ����ڂ̕ω�
        if(gliderState && !shipState)
        {
            //�O���C�_�[���I���ɂ���
            GliderObject.SetActive(true);

            //�D���I�t�ɂ���
            ShipObject.SetActive(false);

           
            //�O���C�_�[�̈ʒu���v���C���[�̈ʒu�ɂ���
            GliderObject.transform.position = Player.transform.position + (Vector3.up * 2.0f);
            change = true;

            //�D�̓����蔻�������
            ShipObject.GetComponent<ShipPositionStatic>().col = false;


            //�V�b�v�ւ̃`�F���W��false
            change = false;
        }
        else if(shipState && !gliderState)
        {
            if(!change)
            {
                //�D�̈ʒu���v���C���[�̈ʒu�ɂ���
                ShipObject.transform.position = Player.transform.position;
                change = true;
            }

            //�O���C�_�[���I�t�ɂ���
            GliderObject.gameObject.SetActive(false);
            //�D���I���ɂ���
            ShipObject.gameObject.SetActive(true);

            //�D�̏ꍇ�p���������ē����蔻���ς���
            Player.GetComponent<BoxCollider>().isTrigger = true;
            MeshObject.gameObject.SetActive(false);
        }
        else
        {
            //�O���C�_�[���I�t�ɂ���
            GliderObject.gameObject.SetActive(false);
            //�D���I�t�ɂ���
            ShipObject.gameObject.SetActive(false);
            //�D�̏ꍇ�p���������ē����蔻���ς���
            Player.GetComponent<BoxCollider>().isTrigger = false;
            MeshObject.gameObject.SetActive(true);

            //�D�̓����蔻�������
            ShipObject.GetComponent<ShipPositionStatic>().col = false;

            //�V�b�v�ւ̃`�F���W��false
            change = false;
        }
    }

    public void RespownCheckPoint()
    {
        //���Ԃ𓮂���
        Time.timeScale = 1.0f;
        //�`�F�b�N�|�C���g�ɖ߂�
        Player.transform.position = checkPoint.position;
        //��Ԃ̏�����
        humanState = false;
        gliderState = false;
        shipState = false;
        controller.ship = shipState;
        MeshObject.gameObject.SetActive(true);
    }
}
