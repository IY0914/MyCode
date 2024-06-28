using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackMove : MonoBehaviour
{
    //プレイヤーの情報
    private GameObject controller;
  

    private void Start()
    {
        //取得一覧
        controller = GameObject.FindGameObjectWithTag("Player");
      
    }
    // Update is called once per frame
    void Update()
    {
       
            //プレイヤーの横移動量だけを取得
            this.transform.position = new Vector3(controller.transform.position.x, 6.0f, -100.0f);
        
    }
}
