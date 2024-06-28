using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    //移動用リギッド
    Rigidbody rb;
    //アニメーション
    Animator anim;
    //移動スピード
    public float speed = 3.0f;
    //ジャンプ力
    public float JumpPower = 5.0f;
    //キー入力取得用変数
    float InputHorizontal = 0.0f;
    float InputVertical = 0.0f;
    float InputSpace = 0.0f;
    //歩行キーが押されていたら
    bool WalkKey = false;
    bool JumpKey = false;
    bool AttackKey = false;
    

    void Start()
    {
        rb = GetComponent<Rigidbody>();
        anim = GetComponent<Animator>();
    }

    void Update()
    {
        AttackKey = false;
        //{カメラの位置に基づいてプレイヤーの向きを決めて移動}
        //------------------------------------------------------------------------------------------------------//
        ////横移動入力
        //InputHorizontal = Input.GetAxisRaw("Horizontal");
        ////縦移動入力 
        //InputVertical = Input.GetAxisRaw("Vertical");
        ////ジャンプ入力
        //if (Input.GetKey(KeyCode.Space))
        //{
        //    JumpKey = true;
        //}
        //攻撃入力
        if(Input.GetMouseButton(0))
        {
            AttackKey = true;
        }
        //------------------------------------------------------------------------------------------------------//


        AnimationController();
        //m_TurnAmount = Mathf.Atan2(move.x, move.z);
    }

    private void AnimationController()
    {

        anim.SetBool("Attack", AttackKey);
        //anim.SetFloat("Forward", rb.velocity.z, 0.1f, Time.deltaTime);
        //anim.SetFloat("Turn", m_TurnAmount, 0.1f, Time.deltaTime);
        //anim.SetBool("Crouch", m_Crouching);
        //anim.SetBool("OnGround", m_IsGrounded);
        //if (!m_IsGrounded)
        //{
        //    anim.SetFloat("Jump", rb.velocity.y);
        //}
    }

    private void FixedUpdate()
    {
        //{カメラの位置に基づいてプレイヤーの向きを決めて移動}
        //------------------------------------------------------------------------------------------------------//
        //カメラ方向を基準に方向ベクトルの取得
        Vector3 cameraFoward = Vector3.Scale(Camera.main.transform.forward, new Vector3(1, 0, 1)).normalized;

        //移動入力とカメラの方向を基準に方向決定(横+縦)
        Vector3 moveForward = Camera.main.transform.right * InputHorizontal + cameraFoward * InputVertical;

        //移動方向にスピードをかけて速度を出す,ジャンプなどがある場合のy軸加算
        rb.velocity = moveForward * speed + new Vector3(0, rb.velocity.y, 0);

        //キャラクターの向きを進行方向
        if (moveForward != Vector3.zero)
        {
            transform.rotation = Quaternion.LookRotation(moveForward);
        }
        //------------------------------------------------------------------------------------------------------//
    }
}