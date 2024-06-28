using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PositionBlock : MonoBehaviour
{
    //基準になるオブジェクト
    private GameObject homeObject;

    private void Start()
    {
        //基準になるオブジェクトの情報
        homeObject = GameObject.Find("PlayerStateCot");
    }
    // Update is called once per frame
    void Update()
    {
        //基準よりも1.5cm上に置く
        this.transform.position = homeObject.transform.position + (Vector3.up * 1.5f);
    }
}
