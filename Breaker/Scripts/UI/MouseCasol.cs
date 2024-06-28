using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseCasol : MonoBehaviour
{

    // Update is called once per frame
    void Update()
    {
        //マウスの追尾
        this.gameObject.transform.position = Input.mousePosition;
        //マウスの非表示
        Cursor.visible = false;
    }
}
