using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoulEffect : MonoBehaviour
{
    //パーティクルを取得
    public ParticleSystem particleSystem;
    public GameObject resultPanel;

    private void OnTriggerEnter(Collider other)
    {

        if(other.gameObject.tag == "Player")
        {
            //再生
            particleSystem.Play();
            //ゴールオブジェクトの削除
            Destroy(this.gameObject);

            //プレイヤーのコントロールを切る
            other.gameObject.GetComponent<PlayerController>().Controlller = false;

            resultPanel.SetActive(true);
        }
    }
}
