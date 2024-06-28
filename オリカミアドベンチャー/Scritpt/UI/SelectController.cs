using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectController : MonoBehaviour
{
    /*-----------�X�e�[�W�Z���N�g�̕ϐ�----------*/
    //�A�j���[�V�����p�ϐ�
    public bool runanimONOFF = false;
    //�X�e�[�W�̐����擾
    private SelectStage selectStage;
    //�A�j���[�V�������
    private Animator anim;
    //�p�[�e�B�N���V�X�e��
    private ParticleSystem particleSystem;
    //�ړ��O�ʒu
    private Vector3 beforePos = Vector3.zero;
    //�ړ���ʒu
    private Vector3 afterPos = Vector3.zero;
    //�ړ������̐ݒ�
    private int direction;
    //���`�⊮�p�ϐ�
    private float lerpValue = 0.0f;
    //�ړ����̃t���O
    private bool moveFlag;

    // Start is called before the first frame update
    void Start()
    {
        //�Z���N�g�X�e�[�W�̎擾
        selectStage = GameObject.Find("StagePoint").GetComponent<SelectStage>();  
        //�A�j���[�V�������擾
        anim = this.gameObject.GetComponent<Animator>();
        //�p�[�e�B�N�����擾
        particleSystem = this.gameObject.GetComponent<ParticleSystem>();
        //������0��
        direction = 0;
        //�t���O�̏�����
        moveFlag = false;
    }

    // Update is called once per frame
    void Update()
    {
        //���E�L�[�ł̓���
        if (Input.GetKeyDown(KeyCode.RightArrow) && !moveFlag)
        {
            MoveDirection(1);
        }
        if (Input.GetKeyDown(KeyCode.LeftArrow) && !moveFlag)
        {
            MoveDirection(-1);
        }

        //�ړ��t���O�ŊǗ�
        if (moveFlag)
        {
            //�L�����N�^�[�̈ړ�
            AddMovePlayerCharacter();
        }
        else
        {
            //�A�j���[�V�����̐ݒ�
            anim.SetBool("Running", false);
            anim.SetBool("Idol", true);
            //�p�[�e�B�N���̒�~
            particleSystem.Stop();
            //���������Z�b�g
            direction = 0;
            //�⊮��������
            lerpValue = 0.0f;
        }
    }

    //�ړ��������^���I�Ɏ擾����(direction = 1 �` �[�P)
    void MoveDirection(int dir)
    {
        //���݂̃X�e�[�W��
        int stageNum = selectStage.stageObject.Count;

        //�X�e�[�W�̍ő吔�������Ȃ��Ȃ�
        if(selectStage.stageNumber < stageNum)
        {
            //�ԍ��������O�ɂȂ�Ȃ�߂�
            if ((selectStage.stageNumber == 0 && dir == -1 )||
                (selectStage.stageNumber == 3 && dir == 1) )
                return;

            //�t���O�̐؂�ւ�
            moveFlag = true;

            //����A�j���[�V�������Đ�����
            anim.SetBool("Running", moveFlag);
            //����
            direction = dir;
            //���݈ʒu����
            beforePos = this.transform.position;
            //�ڕW���W��ݒ�
            afterPos = selectStage.stageObject[selectStage.stageNumber + dir].transform.position;
        }
    }

    void AddMovePlayerCharacter()
    {
        //�p�[�e�B�N���̍Đ�
        particleSystem.Play();

        lerpValue += Time.deltaTime;

        //���`�⊮
        transform.position = Vector3.Lerp(beforePos, afterPos, lerpValue);

        //�⊮������������t���O��؂�ւ���
        if (lerpValue > 1.0f) moveFlag = false;
    }
}
