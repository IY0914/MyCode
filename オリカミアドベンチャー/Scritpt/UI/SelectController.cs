using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectController : MonoBehaviour
{
    /*-----------ステージセレクトの変数----------*/
    //アニメーション用変数
    public bool runanimONOFF = false;
    //ステージの数を取得
    private SelectStage selectStage;
    //アニメーション情報
    private Animator anim;
    //パーティクルシステム
    private ParticleSystem particleSystem;
    //移動前位置
    private Vector3 beforePos = Vector3.zero;
    //移動先位置
    private Vector3 afterPos = Vector3.zero;
    //移動方向の設定
    private int direction;
    //線形補完用変数
    private float lerpValue = 0.0f;
    //移動中のフラグ
    private bool moveFlag;

    // Start is called before the first frame update
    void Start()
    {
        //セレクトステージの取得
        selectStage = GameObject.Find("StagePoint").GetComponent<SelectStage>();  
        //アニメーションを取得
        anim = this.gameObject.GetComponent<Animator>();
        //パーティクルを取得
        particleSystem = this.gameObject.GetComponent<ParticleSystem>();
        //方向を0に
        direction = 0;
        //フラグの初期化
        moveFlag = false;
    }

    // Update is called once per frame
    void Update()
    {
        //左右キーでの動き
        if (Input.GetKeyDown(KeyCode.RightArrow) && !moveFlag)
        {
            MoveDirection(1);
        }
        if (Input.GetKeyDown(KeyCode.LeftArrow) && !moveFlag)
        {
            MoveDirection(-1);
        }

        //移動フラグで管理
        if (moveFlag)
        {
            //キャラクターの移動
            AddMovePlayerCharacter();
        }
        else
        {
            //アニメーションの設定
            anim.SetBool("Running", false);
            anim.SetBool("Idol", true);
            //パーティクルの停止
            particleSystem.Stop();
            //方向をリセット
            direction = 0;
            //補完を初期化
            lerpValue = 0.0f;
        }
    }

    //移動方向を疑似的に取得する(direction = 1 ～ ー１)
    void MoveDirection(int dir)
    {
        //現在のステージ数
        int stageNum = selectStage.stageObject.Count;

        //ステージの最大数よりも少ないなら
        if(selectStage.stageNumber < stageNum)
        {
            //番号が幅内外になるなら戻す
            if ((selectStage.stageNumber == 0 && dir == -1 )||
                (selectStage.stageNumber == 3 && dir == 1) )
                return;

            //フラグの切り替え
            moveFlag = true;

            //走るアニメーションを再生する
            anim.SetBool("Running", moveFlag);
            //方向
            direction = dir;
            //現在位置を代入
            beforePos = this.transform.position;
            //目標座標を設定
            afterPos = selectStage.stageObject[selectStage.stageNumber + dir].transform.position;
        }
    }

    void AddMovePlayerCharacter()
    {
        //パーティクルの再生
        particleSystem.Play();

        lerpValue += Time.deltaTime;

        //線形補完
        transform.position = Vector3.Lerp(beforePos, afterPos, lerpValue);

        //補完が完了したらフラグを切り替える
        if (lerpValue > 1.0f) moveFlag = false;
    }
}
