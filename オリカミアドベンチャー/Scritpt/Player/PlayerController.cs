using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;


public class PlayerController : MonoBehaviour
{
    /*�p�u���b�N�I�u�W�F�N�g*/
    //�ړ����x
    public float speed = 1.0f;
    //������
    public float jumpForce = 10.0f;
    private float jumpTime = 0.0f;
    //�U���ɂ�鐁�����
    public float power = 1000.0f;
    //�W�����v
    public bool jump = false;
    //�v���C���[�̌���
    public int direct = 1;
    //�v���C���[�̗̑�
    public int HP = 3;
    //���G���Ԃ̌v��
    public float invisibleTime = 0.5f;

    /*���*/
    //������
    public bool glider = false;
    //�D���
    public bool ship = false;
    //�U��
    public bool attack = false;
    //����U��
    public bool S_attack = false;


    /*���̑�*/
    //������
    public bool Controlller = true;
    //���̕ێ�
    public int paperNum = 10;

    //�U�����X�g
    public List<string> SpecialAttack;
    public int specialNumber = 0;
    //�����p�̔��e�I�u�W�F�N�g
    public GameObject Bomb;
    public GameObject PaperPlane;
    //�p�[�e�B�N���V�X�e��
    public List<ParticleSystem> ptcs;
    //�X�e�[�g�I�u�W�F�N�g
    public PlayerState state;
    //�Փ�
    public GameObject impact;

    //���ʉ�
    private AudioSource se_clip;

    /*�v���C�x�[�g�I�u�W�F�N�g*/
    //����
    private Rigidbody rigidBody;
    //�N�[���^�C��
    public float specialCool = 0.0f;
    public float coolTime = 0.0f;
    //���̕�[����
    private float chargeTime = 0.0f;
    //�U���Ώۂ̏��
    private GameObject enemyInfo;
    private GameObject gimmicInfo;
    //�v���C���[�̍s���ɑ΂����
    private Vector3 PlayerVelocity = Vector3.zero;
    //�D�̕������������炤
    private Vector3 ShipVelocity = Vector3.zero;


    // Start is called before the first frame update
    void Start()
    {
        //�擾
       rigidBody = GetComponent<Rigidbody>();
        //�擾
        se_clip = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (HP <= 0) Controlller = false;

        //�s�������邩
        if(Controlller)
        {
            //�N�[���^�C��������
            if(coolTime > 0.0f) coolTime -= Time.deltaTime;
            else coolTime = 0.0f;
            if (specialCool > 0.0f) specialCool -= Time.deltaTime;
            else specialCool = 0.0f;

            //�ړ����Ȃ��ꍇ�����̏���؂�
            direct = 0;

            //�D�̏��(�얔�͐��ɐG�ꂽ�ꍇ)
            if (ship && !glider) ShipStateMove();
            else HumanGliderMove();

            //�U��
            if (Input.GetKey(KeyCode.F) && coolTime == 0.0f
               && paperNum > 0)
            {
                //�U�����
                Attack();
                //�N�[���^�C���Ɏ��Ԃ��w��
                coolTime = 1.0f;

                attack = true;
            }
            else
            {
                attack = false;
            }

            //���G���Ԃ̊֐�
            InvisibleTimeCount();

            //����ȍU���̐؂�ւ�
            ChengeSpecialAttack();

            //����ȍU��
            if (Input.GetKeyDown(KeyCode.E) && specialCool == 0.0f
                && !S_attack) SpacialAttack();

            //���̕�[�֐�
            if (paperNum <= 5 && paperNum >= 0) AutoPaperCharge();  //�T���ȉ��O���ȏ�
            else if (paperNum > 25) paperNum = 25;                  //�Q�T���ȓ��Ɏ��߂�
            else if (paperNum < 0) paperNum = 0;                                      //�O�����������ꍇ�͂O�Ɏ��߂�
        }

        if (PlayerVelocity.y != 0.0f) ptcs[0].Pause();
    }

    private void Attack()
    {
        //����炷����
        se_clip.Play();

        //�A�j���[�V����


        //���̖��������炷
        paperNum--;

        Vector3 direct = Vector3.zero;

        if (enemyInfo != null)
        {
            //�U��������������ɑ΂��Đ�����΂�
            direct = transform.position - enemyInfo.transform.position;
            if (direct.normalized.x < 0)
            {
                enemyInfo.GetComponent<Rigidbody>().AddForce(power, 0.0f, 0.0f);
            }
            else if (direct.normalized.x > 0)
            {
                enemyInfo.GetComponent<Rigidbody>().AddForce(-power, 0.0f, 0.0f);
            }
            //�̗͂����炷
            enemyInfo.GetComponent<EnemyLife>().Life--;
        }

        //�U���G�t�F�N�g
        ptcs[1].Play();
    }

    //����U��
    private void SpacialAttack()
    {
        //�U���̏��
        if (SpecialAttack[specialNumber] == "Bomb" && paperNum >= 10)
        {
            //���e�̐���
            GameObject gimmic = GameObject.Instantiate(Bomb, this.transform.position + new Vector3(1.0f,1.0f,0.0f), this.transform.rotation);

            //�����Ă�������ɑ΂��Đ�����΂�
            Vector3 direct = transform.position;

            //�����ɉ������Ռ��̏���
            if (direct.normalized.x < 0) gimmic.GetComponent<Rigidbody>().AddForce(1000.0f, 30.0f, 0.0f);
            else if (direct.normalized.x > 0) gimmic.GetComponent<Rigidbody>().AddForce(-1000.0f, 30.0f, 0.0f);
            else gimmic.GetComponent<Rigidbody>().AddForce(1000.0f, 30.0f, 0.0f);

            //��������
            paperNum -= 10;
            //�N�[���^�C���̃��Z�b�g
            specialCool = 3.0f;

        }
        else if (SpecialAttack[specialNumber] == "Plane" && paperNum >= 15)
        {
            //�����ʒu
            Vector3 pos = transform.position + new Vector3(0.0f, 6.0f, 0.0f);
            //����s�@�̐���
            GameObject.Instantiate(PaperPlane, pos, this.transform.rotation);

            //���̏���
            paperNum -= 15;
            //�N�[���^�C���̃��Z�b�g
            specialCool = 3.0f;
        }
    }

    //�l�̏�ԂƊ����Ԃ̈ړ�
    private void HumanGliderMove()
    {
        rigidBody.useGravity = true;
       // this.gameObject.GetComponent<Mesh>().

        //�ړ�����Ƒ��x���Z
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            direct = -1;
            rigidBody.velocity = new Vector3(-speed, rigidBody.velocity.y, 0.0f);
            transform.eulerAngles = new Vector3(0.0f, 135.5f, 0.0f);
            //�p�[�e�B�N���̊J�n
            ptcs[0].Play();
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            direct = 1;
            rigidBody.velocity = new Vector3(speed, rigidBody.velocity.y, 0.0f);
            //transform.Rotate(0, direct, 0);
            transform.eulerAngles = new Vector3(0.0f, 45.0f, 0.0f);
            //�p�[�e�B�N���̊J�n
            ptcs[0].Play();
        }

        //�W�����v�̔���
        if (Input.GetKeyDown(KeyCode.Space)) JumpState();
    }

    //�D�̏��
    private void ShipStateMove()
    {
        //�d�͂�؂�
        rigidBody.useGravity = false;

        bool staticShip = state.ShipObject.GetComponent<ShipPositionStatic>().col;

        //�ړ�����Ƒ��x���Z
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            direct = -1;
            ShipVelocity = new Vector3(-speed, rigidBody.velocity.y, 0.0f);
            transform.eulerAngles = new Vector3(0.0f, 180.0f, 0.0f);   
        }
        else if(Input.GetKey(KeyCode.RightArrow))
        {
            direct = 1;
            ShipVelocity = new Vector3(speed, rigidBody.velocity.y, 0.0f);
            //transform.Rotate(0, direct, 0);
            transform.eulerAngles = new Vector3(0.0f, 45.0f, 0.0f);
        }
        else
        {
            //���x�������|����
            ShipVelocity *= 0.98f;
        }

        //���x�����ȉ��ɐ�������~�߂�
        if(ShipVelocity.magnitude <= 0.001f)
        {
            ShipVelocity.x = 0.0f;
        }

        //�D���n�ʂɐڐG���Ă����瓮���Ȃ�
        if (staticShip)
        {
            ShipVelocity = Vector3.zero;
        }

        //�������Z
        state.ShipObject.GetComponent<Rigidbody>().velocity = ShipVelocity;
        rigidBody.velocity = ShipVelocity;

        //�X�y�[�X����������ԂɎ��Ԍv��
        if (Input.GetKey(KeyCode.Space)) JumpState();
    }

    //�O���C�_�[&�W�����v����
    private void JumpState()
    {
        //�n�ʂɒ��n���Ă�����
        if(!glider && !jump)
        {
            //�W�����v��ON�ɂ���
            jump = true;
            glider = false;
            
            //�D�̏��
            if(ship)
            {
                rigidBody.useGravity = true;
                rigidBody.velocity = new Vector3(rigidBody.velocity.x, jumpForce * 2.0f, 0.0f);
                ship = false;
            }
            //�D�̏�Ԃ��łȂ��Ȃ�W�����v����
            else
            {
                rigidBody.velocity = new Vector3(rigidBody.velocity.x, jumpForce, 0.0f);
            }
        }
        else if (jump && !glider)
        {
            glider = true;
            rigidBody.drag = 10.0f;
        }
        else
        {
            glider = false;
            rigidBody.drag = 0.0f;
        }

        //�W�����v���͐؂�
        //�p�[�e�B�N���̏I��
        ptcs[0].Pause();
    }

    //����U���̐؂�ւ��֐�
    void ChengeSpecialAttack()
    {
        //�ԍ��̑���
        if (Input.GetKeyDown(KeyCode.D)) specialNumber++;
        else if (Input.GetKeyDown(KeyCode.A))specialNumber--;

        if(specialNumber > SpecialAttack.Count - 1) specialNumber = SpecialAttack.Count - 1;
        else if(specialNumber < 0) specialNumber = 0;
    }

    //�܂莆�̎�����[�̊֐�
    void AutoPaperCharge()
    {
        //���Ԃ̉��Z
        chargeTime += Time.deltaTime;

        //1.5�b����1��
        if(chargeTime >= 3.0f)
        {
            paperNum++;
            //���Ԃ����Z�b�g
            chargeTime = 0;
        }
       
    }

    //���G���Ԃ��v������֐�
    private void InvisibleTimeCount()
    {
        //���Z
        invisibleTime -= Time.deltaTime;

        //�O�ȉ��ɂȂ�Ȃ��悤��
        if(invisibleTime < 0.0f)
        {
            invisibleTime = 0.0f;
        }
    }

    private void OnCollisionStay(Collision collision)
    {
        if(collision.gameObject.tag == "Ground")
        {
            jump = false;
            glider = false;
            ship = false;
            rigidBody.drag = 0.0f;



            Controlller = true;
        }

        if(collision.gameObject.tag == "Lake")
        {
            jump = false;
            glider = false;
            ship = true;
            rigidBody.drag = 0.0f;
        }

        //�G�ƐڐG������
        if (collision.gameObject.tag == "Enemy" && invisibleTime == 0.0f)
        {
            //�q�b�g�|�C���g�����炷
            HP--;
            //����������֐�����΂�(��)
            this.GetComponent<Rigidbody>().AddForce(-500.0f, 0.0f, 0.0f);
            //���G���Ԃ̃��Z�b�g
            invisibleTime = 0.5f;

            //�G�t�F�N�g���o��
            impact.transform.position = collision.contacts[0].point;
            impact.GetComponent<ParticleSystem>().Play();
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == "Enemy")
        {
            //�U���̌���
            Vector2 direction = other.gameObject.transform.position - this.transform.position;

            //������͈͂Ȃ�U���\
            if (direction.magnitude >= GetComponent<SphereCollider>().radius)
            {
                Debug.Log("�U���\");
                enemyInfo = other.gameObject;
            }
        }
    }
}
