using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TimeLimitTex : MonoBehaviour
{
    //埋め合わせ用画像
    public Image timeFillAmount;
    //制限時間
    public float timeLimit;
    //0.1あたりの角度
    private float radian = 0;
    //fillの値
    private float fillNum = 0;
    //時間経過
    private float time = 0;

    // Start is called before the first frame update
    void Start()
    {
        //画像埋め合わせの数値
        fillNum = 1.0f / (timeLimit * 60.0f);

        //マイナス値
        radian = -360.0f / (timeLimit * 60.0f);
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;

        Vector3 rotation = new Vector3(0.0f, 0.0f, radian * Time.deltaTime);

        timeFillAmount.fillAmount = fillNum * time;

        transform.Rotate(rotation);
    }
}
