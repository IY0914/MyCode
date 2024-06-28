using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BombScript : MonoBehaviour
{
    //パーティクル
    public GameObject particle;
    //マテリアル
    MeshRenderer material;
    //起爆時間
    public float detonationTime = 3.0f;
    //色時間
    public float timeR = 0.0f;

    private void Start()
    {
        material = GetComponent<MeshRenderer>();
    }

    private void Update()
    {
        //時間加算
        detonationTime -= Time.deltaTime;
        //3秒で起爆する
        if(detonationTime < 0.0f)
        {
            Detonaton();
        }

        timeR =  (255.0f / 3.0f) * Time.deltaTime;

        //起爆時間を色でわかるようにするs
        material.material.color = material.material.color + new Color32((byte)(timeR),0,0,0);

    }

    //起爆関数
    void Detonaton()
    {
        //消去
        Destroy(this.gameObject);
        //パーティクルオブジェクトの生成
        Instantiate(particle, this.transform.position, this.transform.rotation);
    }

    private void OnCollisionEnter(Collision collision)
    {
        //敵に当たった場合
        if (collision.gameObject.tag == "Enemy")
        {
            Detonaton();
        }
    }
}
