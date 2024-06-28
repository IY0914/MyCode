using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class EnemyLife : MonoBehaviour
{
    //体力
    public int Life = 3;
    //スライダー
    public Slider slider;
    //位置情報
    private Vector3 pos;

    //プレイヤーを参照
    PlayerController playerController;
    //エフェクト
    ParticleSystem particle;

    // Start is called before the first frame update
    void Start()
    {
        //スライダーの最大値を変える
        slider.maxValue = Life;
        //スライダーをマックスにする
        slider.value = Life;
        //取得
        playerController = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
        //パーティクルの取得
        particle = this.GetComponent<ParticleSystem>();
    }

    // Update is called once per frame
    void Update()
    {
        //スライダーの増減
        slider.value = Life;
        //HPが無くなったら消す
        if (Life <= 0) Destroy(this.gameObject);
    }

    private void OnCollisionEnter(Collision collision)
    {
        //攻撃ごとに体力を減らす
        if (collision.gameObject.tag == "Gimmic") 
        {
            Life -= 3;

            //接触判定の位置
            pos = collision.contacts[0].point;
        }
    }

}
