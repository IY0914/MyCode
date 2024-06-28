using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShipPositionStatic : MonoBehaviour
{
    //プレイヤーコントローラー
    private PlayerController controller;
    private GameObject player;

    public bool col = false;

    private void Start()
    {
        //取得
        controller = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
        player = GameObject.FindGameObjectWithTag("Player");
    }

    //当たった瞬間
    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Ground")
        {
            col = true;
        }
    }
    //地面と非接触になったら 
    private void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.tag == "Ground")
        {
            col = false;
        }
    }

}
