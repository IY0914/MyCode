using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChageHeart : MonoBehaviour
{
    //空のハートの画像
    public Sprite emptyHeartImage;
    //プレイヤーの情報
    private PlayerController playerState;
    public List<GameObject> images = new List<GameObject>(3);
    //ゲームオーバーUI
    public GameObject gameOverUI;

    private void Start()
    {
        playerState = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //プレイヤーの体力が０より大きい場合
        if(playerState.HP > 0 && playerState.HP < 3)
        { 
            //画像を入れ替える
            images[playerState.HP].GetComponent<Image>().sprite = emptyHeartImage;
        }
        if(playerState.HP <= 0)
        {
            gameOverUI.SetActive(true);
        }
    }
}
