using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Breakble : MonoBehaviour
{
    //壊れる前のオブジェクト
    [SerializeField] private Transform BrokenPrefab;

    //接続しているオブジェクトの情報取得
    Rigidbody JointObjects;

    //接続しているかどうか
    FixedJoint StartJoint;
    FixedJoint nowJoint;
    public bool JointONOFF = true;

    //連鎖破壊switch
    public bool BreakDestroy = false;


    private void Start()
    {
        if (nowJoint == null)
        {
            //Debug.Log("なし");
            JointONOFF = false;
        }
        else
        {
            StartJoint = this.gameObject.GetComponent<FixedJoint>();
        }
    }

    private void Update()
    {
        if(nowJoint)
        nowJoint = this.gameObject.GetComponent<FixedJoint>();

        if (nowJoint == null)
        {
            //Debug.Log("なし");
            JointONOFF = false;
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        // 衝突した相手にPlayerタグが付いているとき
        if (collision.gameObject.tag == "Weapon")
        {
            if (JointONOFF == true)
            {
                if (nowJoint.connectedBody == null)
                {
                    Destroy(nowJoint);
                    JointONOFF = false;
                }

            }


            //壊れたオブジェクトに位置情報と回転情報を
            //与えるためにプレハブ位置づくり

            Debug.Log("当たった");

            //つながっているときに
            if (JointONOFF)
            {
                //コネクトしたオブジェクトの物理演算（リジッドボディ）を取得
                JointObjects = nowJoint.connectedBody;

                //コネクトしていたオブジェクトに重力をかける
                JointObjects.isKinematic = false;
                JointObjects.useGravity = true;

                //変換した物理挙動をonにする
                foreach (Rigidbody rb in BrokenPrefab.GetComponentsInChildren<Rigidbody>())
                {
                    rb.isKinematic = false;
                    JointObjects.GetComponent<GameObject>().GetComponent<Breakble>().BreakDestroy = true;
                }
            }

            Transform brokenTrasform = Instantiate(BrokenPrefab, transform.position, transform.rotation);
            //壊れた物に拡大率を持たせる
            //brokenTrasform.localScale = transform.localScale;

            //子コンポーネントのリギッドボディに力を加えるために取得
            //複数取るのでChildrenにsを付ける
            //foreach (Rigidbody rigitbody in brokenTrasform.GetComponentsInChildren<Rigidbody>())
            //{
            //    //リギッドボディに崩れるときの力を加える
            //    rigitbody.AddExplosionForce(150.0f, transform.position + Vector3.up * 0.5f, 5.0f);

            //}


            //自分の破棄
            Destroy(gameObject);
        }
    }


    // Update is called once per frame

    //private void OnTriggerEnter(Collider other)
    //{
    //    // 衝突した相手にPlayerタグが付いているとき
    //    if (other.gameObject.tag == "Weapon")
    //    {
    //        //壊れたオブジェクトに位置情報と回転情報を
    //        //与えるためにプレハブ位置づくり

    //        Transform brokenTrasform = Instantiate(BrokenPrefab, transform.position, transform.rotation);
    //        //壊れた物に拡大率を持たせる
    //        //brokenTrasform.localScale = transform.localScale;

    //        //子コンポーネントのリギッドボディに力を加えるために取得
    //        //複数取るのでChildrenにsを付ける
    //        foreach (Rigidbody rigitbody in brokenTrasform.GetComponentsInChildren<Rigidbody>())
    //        {
    //            //当たったところを計算
    //            Vector3 TriggerPoint = other.ClosestPointOnBounds(this.transform.position);

    //            Debug.Log(TriggerPoint);

    //            //リギッドボディに崩れるときの力を加える
    //            rigitbody.AddExplosionForce(150.0f, transform.position + Vector3.up * 0.5f, 5.0f);

    //        }

    //        //自分の破棄
    //        Destroy(gameObject);
    //    }
    //}

}
