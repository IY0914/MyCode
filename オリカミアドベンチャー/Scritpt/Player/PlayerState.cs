using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerState : MonoBehaviour
{
    //状態を色で表す(仮)
    Material material;
    //プレイヤーコントローラー
    public PlayerController controller;
    //プレイヤーオブジェクト
    public GameObject Player;

    //船状態
    public GameObject ShipObject;
    //グライダー状態
    public GameObject GliderObject;
    //メッシュ
    public GameObject MeshObject;

    //状態の遷移を取得
    public bool humanState = false;
    public bool gliderState = false;
    public bool shipState = false;

    //チェックポイント
    public Transform checkPoint;

    //位置の更新をbool
    private bool change = false;

    // Start is called before the first frame update
    void Start()
    {
        //取得
        material = this.gameObject.GetComponent<Material>();
    }

// Update is called once per frame
void Update()
    {
        //各状態を取得
        gliderState = controller.glider;
        shipState = controller.ship;

        //一定以上落ちるとチェックポイントに戻る
        if(Player.transform.position.y < -5.0f)
        {
            Player.transform.position = checkPoint.position;
            //体力を削る
            Player.GetComponent<PlayerController>().HP -= 1;

        }

        if(controller.invisibleTime != 0.0f) MeshObject.SetActive(false);

        //状態の遷移と見た目の変化
        if(gliderState && !shipState)
        {
            //グライダーをオンにする
            GliderObject.SetActive(true);

            //船をオフにする
            ShipObject.SetActive(false);

           
            //グライダーの位置をプレイヤーの位置にする
            GliderObject.transform.position = Player.transform.position + (Vector3.up * 2.0f);
            change = true;

            //船の当たり判定を消す
            ShipObject.GetComponent<ShipPositionStatic>().col = false;


            //シップへのチェンジをfalse
            change = false;
        }
        else if(shipState && !gliderState)
        {
            if(!change)
            {
                //船の位置をプレイヤーの位置にする
                ShipObject.transform.position = Player.transform.position;
                change = true;
            }

            //グライダーをオフにする
            GliderObject.gameObject.SetActive(false);
            //船をオンにする
            ShipObject.gameObject.SetActive(true);

            //船の場合姿をかくして当たり判定を変える
            Player.GetComponent<BoxCollider>().isTrigger = true;
            MeshObject.gameObject.SetActive(false);
        }
        else
        {
            //グライダーをオフにする
            GliderObject.gameObject.SetActive(false);
            //船をオフにする
            ShipObject.gameObject.SetActive(false);
            //船の場合姿をかくして当たり判定を変える
            Player.GetComponent<BoxCollider>().isTrigger = false;
            MeshObject.gameObject.SetActive(true);

            //船の当たり判定を消す
            ShipObject.GetComponent<ShipPositionStatic>().col = false;

            //シップへのチェンジをfalse
            change = false;
        }
    }

    public void RespownCheckPoint()
    {
        //時間を動かす
        Time.timeScale = 1.0f;
        //チェックポイントに戻る
        Player.transform.position = checkPoint.position;
        //状態の初期化
        humanState = false;
        gliderState = false;
        shipState = false;
        controller.ship = shipState;
        MeshObject.gameObject.SetActive(true);
    }
}
