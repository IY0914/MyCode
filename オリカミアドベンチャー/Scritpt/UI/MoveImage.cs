using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class MoveImage : MonoBehaviour
{
    //位置の変数
    private Vector3 nowPos;
    public GameObject movePos;
    //補間関数のON
    private bool lerpFlag = false;

    //補完数字
    private float time;

    // Start is called before the first frame update
    void Start()
    {
        //最初の位置を設定
        nowPos = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        //移動フラグで移動
        if(lerpFlag)
        {
            time += Time.deltaTime;
        }
        else if(!lerpFlag && time > 0.0f)
        {
            time -= Time.deltaTime;
        }

        //1以上の時０以下の時
        if (time > 1) time = 1;
        else if (time < 0) time = 0;

        transform.position = Vector3.Lerp(nowPos, movePos.transform.position, time);
    }

    //フラグの管理
    public void ChangedFlags()
    {
        if (lerpFlag) lerpFlag = false;
        else lerpFlag = true;
    }
}
