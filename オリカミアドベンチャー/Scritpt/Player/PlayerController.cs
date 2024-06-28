using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;


public class PlayerController : MonoBehaviour
{
    /*パブリックオブジェクト*/
    //移動速度
    public float speed = 1.0f;
    //跳躍力
    public float jumpForce = 10.0f;
    private float jumpTime = 0.0f;
    //攻撃による吹き飛び
    public float power = 1000.0f;
    //ジャンプ
    public bool jump = false;
    //プレイヤーの向き
    public int direct = 1;
    //プレイヤーの体力
    public int HP = 3;
    //無敵時間の計測
    public float invisibleTime = 0.5f;

    /*状態*/
    //滑空状態
    public bool glider = false;
    //船状態
    public bool ship = false;
    //攻撃
    public bool attack = false;
    //特殊攻撃
    public bool S_attack = false;


    /*その他*/
    //操作状態
    public bool Controlller = true;
    //紙の保持
    public int paperNum = 10;

    //攻撃リスト
    public List<string> SpecialAttack;
    public int specialNumber = 0;
    //生成用の爆弾オブジェクト
    public GameObject Bomb;
    public GameObject PaperPlane;
    //パーティクルシステム
    public List<ParticleSystem> ptcs;
    //ステートオブジェクト
    public PlayerState state;
    //衝突
    public GameObject impact;

    //効果音
    private AudioSource se_clip;

    /*プライベートオブジェクト*/
    //物理
    private Rigidbody rigidBody;
    //クールタイム
    public float specialCool = 0.0f;
    public float coolTime = 0.0f;
    //紙の補充時間
    private float chargeTime = 0.0f;
    //攻撃対象の情報
    private GameObject enemyInfo;
    private GameObject gimmicInfo;
    //プレイヤーの行動に対する力
    private Vector3 PlayerVelocity = Vector3.zero;
    //船の物理挙動をもらう
    private Vector3 ShipVelocity = Vector3.zero;


    // Start is called before the first frame update
    void Start()
    {
        //取得
       rigidBody = GetComponent<Rigidbody>();
        //取得
        se_clip = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (HP <= 0) Controlller = false;

        //行動をするか
        if(Controlller)
        {
            //クールタイムを減少
            if(coolTime > 0.0f) coolTime -= Time.deltaTime;
            else coolTime = 0.0f;
            if (specialCool > 0.0f) specialCool -= Time.deltaTime;
            else specialCool = 0.0f;

            //移動がない場合向きの情報を切る
            direct = 0;

            //船の状態(川又は水に触れた場合)
            if (ship && !glider) ShipStateMove();
            else HumanGliderMove();

            //攻撃
            if (Input.GetKey(KeyCode.F) && coolTime == 0.0f
               && paperNum > 0)
            {
                //攻撃状態
                Attack();
                //クールタイムに時間を指定
                coolTime = 1.0f;

                attack = true;
            }
            else
            {
                attack = false;
            }

            //無敵時間の関数
            InvisibleTimeCount();

            //特殊な攻撃の切り替え
            ChengeSpecialAttack();

            //特殊な攻撃
            if (Input.GetKeyDown(KeyCode.E) && specialCool == 0.0f
                && !S_attack) SpacialAttack();

            //紙の補充関数
            if (paperNum <= 5 && paperNum >= 0) AutoPaperCharge();  //５枚以下０枚以上
            else if (paperNum > 25) paperNum = 25;                  //２５枚以内に収める
            else if (paperNum < 0) paperNum = 0;                                      //０よりも小さい場合は０に収める
        }

        if (PlayerVelocity.y != 0.0f) ptcs[0].Pause();
    }

    private void Attack()
    {
        //音を鳴らす処理
        se_clip.Play();

        //アニメーション


        //紙の枚数を減らす
        paperNum--;

        Vector3 direct = Vector3.zero;

        if (enemyInfo != null)
        {
            //攻撃時相手を向きに対して吹き飛ばす
            direct = transform.position - enemyInfo.transform.position;
            if (direct.normalized.x < 0)
            {
                enemyInfo.GetComponent<Rigidbody>().AddForce(power, 0.0f, 0.0f);
            }
            else if (direct.normalized.x > 0)
            {
                enemyInfo.GetComponent<Rigidbody>().AddForce(-power, 0.0f, 0.0f);
            }
            //体力を減らす
            enemyInfo.GetComponent<EnemyLife>().Life--;
        }

        //攻撃エフェクト
        ptcs[1].Play();
    }

    //特殊攻撃
    private void SpacialAttack()
    {
        //攻撃の状態
        if (SpecialAttack[specialNumber] == "Bomb" && paperNum >= 10)
        {
            //爆弾の生成
            GameObject gimmic = GameObject.Instantiate(Bomb, this.transform.position + new Vector3(1.0f,1.0f,0.0f), this.transform.rotation);

            //向いている方向に対して吹き飛ばす
            Vector3 direct = transform.position;

            //方向に応じた衝撃の処理
            if (direct.normalized.x < 0) gimmic.GetComponent<Rigidbody>().AddForce(1000.0f, 30.0f, 0.0f);
            else if (direct.normalized.x > 0) gimmic.GetComponent<Rigidbody>().AddForce(-1000.0f, 30.0f, 0.0f);
            else gimmic.GetComponent<Rigidbody>().AddForce(1000.0f, 30.0f, 0.0f);

            //紙を消費
            paperNum -= 10;
            //クールタイムのリセット
            specialCool = 3.0f;

        }
        else if (SpecialAttack[specialNumber] == "Plane" && paperNum >= 15)
        {
            //生成位置
            Vector3 pos = transform.position + new Vector3(0.0f, 6.0f, 0.0f);
            //紙飛行機の生成
            GameObject.Instantiate(PaperPlane, pos, this.transform.rotation);

            //紙の消費
            paperNum -= 15;
            //クールタイムのリセット
            specialCool = 3.0f;
        }
    }

    //人の状態と滑空状態の移動
    private void HumanGliderMove()
    {
        rigidBody.useGravity = true;
       // this.gameObject.GetComponent<Mesh>().

        //移動判定と速度加算
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            direct = -1;
            rigidBody.velocity = new Vector3(-speed, rigidBody.velocity.y, 0.0f);
            transform.eulerAngles = new Vector3(0.0f, 135.5f, 0.0f);
            //パーティクルの開始
            ptcs[0].Play();
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            direct = 1;
            rigidBody.velocity = new Vector3(speed, rigidBody.velocity.y, 0.0f);
            //transform.Rotate(0, direct, 0);
            transform.eulerAngles = new Vector3(0.0f, 45.0f, 0.0f);
            //パーティクルの開始
            ptcs[0].Play();
        }

        //ジャンプの判定
        if (Input.GetKeyDown(KeyCode.Space)) JumpState();
    }

    //船の状態
    private void ShipStateMove()
    {
        //重力を切る
        rigidBody.useGravity = false;

        bool staticShip = state.ShipObject.GetComponent<ShipPositionStatic>().col;

        //移動判定と速度加算
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
            //速度減速を掛ける
            ShipVelocity *= 0.98f;
        }

        //速度が一定以下に成ったら止める
        if(ShipVelocity.magnitude <= 0.001f)
        {
            ShipVelocity.x = 0.0f;
        }

        //船が地面に接触していたら動かない
        if (staticShip)
        {
            ShipVelocity = Vector3.zero;
        }

        //物理加算
        state.ShipObject.GetComponent<Rigidbody>().velocity = ShipVelocity;
        rigidBody.velocity = ShipVelocity;

        //スペースを押し入る間に時間計測
        if (Input.GetKey(KeyCode.Space)) JumpState();
    }

    //グライダー&ジャンプ判定
    private void JumpState()
    {
        //地面に着地していたら
        if(!glider && !jump)
        {
            //ジャンプをONにする
            jump = true;
            glider = false;
            
            //船の状態
            if(ship)
            {
                rigidBody.useGravity = true;
                rigidBody.velocity = new Vector3(rigidBody.velocity.x, jumpForce * 2.0f, 0.0f);
                ship = false;
            }
            //船の状態をでないならジャンプする
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

        //ジャンプ中は切る
        //パーティクルの終了
        ptcs[0].Pause();
    }

    //特殊攻撃の切り替え関数
    void ChengeSpecialAttack()
    {
        //番号の増減
        if (Input.GetKeyDown(KeyCode.D)) specialNumber++;
        else if (Input.GetKeyDown(KeyCode.A))specialNumber--;

        if(specialNumber > SpecialAttack.Count - 1) specialNumber = SpecialAttack.Count - 1;
        else if(specialNumber < 0) specialNumber = 0;
    }

    //折り紙の自動補充の関数
    void AutoPaperCharge()
    {
        //時間の加算
        chargeTime += Time.deltaTime;

        //1.5秒枚に1枚
        if(chargeTime >= 3.0f)
        {
            paperNum++;
            //時間をリセット
            chargeTime = 0;
        }
       
    }

    //無敵時間を計測する関数
    private void InvisibleTimeCount()
    {
        //減算
        invisibleTime -= Time.deltaTime;

        //０以下にならないように
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

        //敵と接触したら
        if (collision.gameObject.tag == "Enemy" && invisibleTime == 0.0f)
        {
            //ヒットポイントを減らす
            HP--;
            //自分を後方へ吹き飛ばす(仮)
            this.GetComponent<Rigidbody>().AddForce(-500.0f, 0.0f, 0.0f);
            //無敵時間のリセット
            invisibleTime = 0.5f;

            //エフェクトを出す
            impact.transform.position = collision.contacts[0].point;
            impact.GetComponent<ParticleSystem>().Play();
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == "Enemy")
        {
            //攻撃の向き
            Vector2 direction = other.gameObject.transform.position - this.transform.position;

            //当たる範囲なら攻撃可能
            if (direction.magnitude >= GetComponent<SphereCollider>().radius)
            {
                Debug.Log("攻撃可能");
                enemyInfo = other.gameObject;
            }
        }
    }
}
