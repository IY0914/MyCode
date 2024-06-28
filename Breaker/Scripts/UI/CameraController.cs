using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    //追従用
    //追従するオブジェクト
    public GameObject targetObject;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //一人称視点
        this.gameObject.transform.position = targetObject.transform.position;
        this.gameObject.transform.rotation = targetObject.transform.rotation;
    }
}
