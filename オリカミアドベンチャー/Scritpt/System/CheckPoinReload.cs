using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckPoinReload : MonoBehaviour
{
    //通過して更新したか
    public bool check = false;

    //ステートオブジェクト
    private PlayerState playerState;

    private void Start()
    {
        playerState = GameObject.Find("PlayerStateCot").GetComponent<PlayerState>();
    }

    private void OnTriggerEnter(Collider other)
    {
        //一度通った場合は処理しない
        if (check) return;

        //プレイヤーの更新
        if (other.gameObject.tag == "Player")
        {
            //紙の補充
            other.gameObject.GetComponent<PlayerController>().paperNum += 10;
            //ステートのチェックポイントの切り替え
            playerState.checkPoint.position = this.transform.position;

            //判定を切り替える
            check = true;
        }
    }
}
