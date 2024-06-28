using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Assets.Scripts
{
    public class WeaponCollider : MonoBehaviour
    {
        private float impulseMagnitude;

        // Start is called before the first frame update
        void Start()
        {
            //衝突時に発生させる力積の大きさ
            this.impulseMagnitude = 70.0f;
        }

        // Update is called once per frame
        void Update()
        {

        }

        private void OnCollisionEnter(Collision collision)
        {
            //衝突相手はRigidbodyをアタッチした立方体で、別途空から降らせる
            var rigid = collision.gameObject.GetComponent<Rigidbody>();

            //衝突相手の座標から回転中心の座標
            var impulse = (rigid.position - transform.parent.position).normalized * this.impulseMagnitude;

            //瞬間的な衝撃を書ける場合(力積を加える場合)はForceMode.Impulseを使う
            rigid.AddForce(impulse, ForceMode.Impulse);
        }
    }
}
