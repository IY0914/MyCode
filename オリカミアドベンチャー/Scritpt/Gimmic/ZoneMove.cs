using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class ZoneMove : MonoBehaviour
{
  
    //ゾーンの取得
    public GameObject arriveZone;
    //吹き飛ばし対象情報
    private GameObject? targetObject = null;

    //吹き飛ばし判定
    private bool thorow = false;

    /// <summary>
    /// 射出角度
    /// </summary>
    [SerializeField, Range(0F, 90F), Tooltip("射出する角度")]
    private float ThrowingAngle;

   
    private void Start()
    {
        //プレイヤーのオブジェクト取得
        targetObject = GameObject.FindGameObjectWithTag("Player");
        
    }

    private void Update()
    {
       

        //吹き飛ばし判定がONになったら
        if (thorow)
        {
            ThrowObject();
        }

        if (thorow) thorow = false;
    }

    /// <summary>
    /// 乗ったオブジェクトを射出する
    /// </summary>
    private void ThrowObject()
    {
        if (targetObject != null && arriveZone != null)
        {

            // 標的の座標
            Vector3 targetPosition = arriveZone.transform.position;

            // 射出角度
            float angle = ThrowingAngle;

            // 射出速度を算出
            Vector3 velocity = CalculateVelocity(this.transform.position, targetPosition, angle);

            // 射出
            Rigidbody rid = targetObject.GetComponent<Rigidbody>();
            rid.AddForce(velocity * rid.mass, ForceMode.Impulse);
        }
        else
        {
            throw new System.Exception("射出するオブジェクトまたは標的のオブジェクトが未設定です。");
        }
    }

    /// <summary>
    /// 標的に命中する射出速度の計算
    /// </summary>
    /// <param name="pointA">射出開始座標</param>
    /// <param name="pointB">標的の座標</param>
    /// <returns>射出速度</returns>
    private Vector3 CalculateVelocity(Vector3 pointA, Vector3 pointB, float angle)
    {
        // 射出角をラジアンに変換
        float rad = angle * Mathf.PI / 180;

        // 水平方向の距離x
        float x = Vector2.Distance(new Vector2(pointA.x, pointA.z), new Vector2(pointB.x, pointB.z));

        // 垂直方向の距離y
        float y = pointA.y - pointB.y;

        // 斜方投射の公式を初速度について解く
        float speed = Mathf.Sqrt(-Physics.gravity.y * Mathf.Pow(x, 2) / (2 * Mathf.Pow(Mathf.Cos(rad), 2) * (x * Mathf.Tan(rad) + y)));

        if (float.IsNaN(speed))
        {
            // 条件を満たす初速を算出できなければVector3.zeroを返す
            return Vector3.zero;
        }
        else
        {
            return (new Vector3(pointB.x - pointA.x, x * Mathf.Tan(rad), pointB.z - pointA.z).normalized * speed);
        }
    }

    /*
   private void Update()
 {
     //入れ替え先の位置情報
     Vector3 pos = arriveZone.transform.position;

     //入れ替えをする
     if (change_zone)
     {
         targetObject.transform.position = new Vector3(pos.x,pos.y,pos.z);
     }

     //リセット
     change_zone = false;
 }
     */

    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.tag == "Player")
        {
             //入れ替え判定
             thorow = true;

            //操作状態解除
            other.gameObject.GetComponent<PlayerController>().Controlller = false;
            //グライダー状態解除
            other.gameObject.GetComponent<PlayerController>().glider = false;
            //加速度零
            other.gameObject.GetComponent<Rigidbody>().velocity = Vector3.zero;
            //摩擦力をリセット
            other.gameObject.GetComponent<Rigidbody>().drag = 0.0f;

        }

    }
}
