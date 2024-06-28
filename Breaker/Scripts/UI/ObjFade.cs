using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjFade : MonoBehaviour
{

    MeshRenderer Mesh;

    // Start is called before the first frame update
    void Start()
    {
        //マテリアルの取得
        Mesh = GetComponent<MeshRenderer>();
        //透明度をいじる前準備
        StartCoroutine("Transparent");
    }

    IEnumerator Transparent()
    {
        //繰り返しで透明度上昇
        for(int i = 0; i < 255; i ++)
        {
            Mesh.material.color = Mesh.material.color - new Color32(0, 0, 0, 5);
            
            yield return new WaitForSeconds(0.04f);
        }

       
    }
}