using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AttackCoolUI : MonoBehaviour
{
    //イメージのコンポーネント
    private Image image;
    //コントローラー
    private PlayerController playerController;
    //フレームの移動に合わせないために取得
    public GameObject frame;
    
    // Start is called before the first frame update
    void Start()
    {
        //取得
        image = GetComponent<Image>();
        //塗りつぶしを設定
        image.fillAmount = 0;
        //タグで取得
        playerController = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //クールタイムのUIを作成
        image.fillAmount = playerController.specialCool / 3.0f;

        //０以下の場合
        if(image.fillAmount <= 0)
        {
            this.transform.position = frame.transform.position;
        }
    }
}
