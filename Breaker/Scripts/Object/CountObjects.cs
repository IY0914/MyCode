using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CountObjects : MonoBehaviour
{
    //子オブジェクトのオブジェクトを取得
    //Listだと追加が簡単にできる(Unityはこっちで)
    public List<Rigidbody> Parts;
    
    //配列の書き方の場合は既存のオブジェクトを入れておくのには役に立つ？
    //public Rigidbody[] Parts;

    // Update is called once per frame
    void Start()
    {
        //物理演算を取得
        foreach (Rigidbody obj in this.gameObject.GetComponentsInChildren<Rigidbody>())
        {
            Parts.Add(obj);
        }
    }

    private void Update()
    {
        //キネマティックの数を取得
        int kinematicIsFalse = 0;

        //子オブジェクトの数分回す
        for(int i = 0; i < Parts.Count; i++)
        {
            //数を数えて取得
            if (Parts[i].isKinematic)
            {
                kinematicIsFalse++;
                Debug.Log(kinematicIsFalse);
            }
        }
        


        //半分壊れたら全部破壊
        if (kinematicIsFalse <= Parts.Count / 1.5)
        {
            for(int i = 0; i < Parts.Count; i++)
            { 
               Parts[i].isKinematic = false;
            }
        }
    }
}
