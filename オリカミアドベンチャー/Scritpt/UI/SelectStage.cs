using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SelectStage : MonoBehaviour
{
    //ステージのリスト
    public List<GameObject> stageObject;
    //ステージ名リスト
    public List<string> scenes = new List<string>();
    //選択ステージ番号
    public int  stageNumber = 0;
    //キャンバスのコルーチンを動かす
    public TitleButtonFlag flag;

    private void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        //踏んでいるかを取得
        for(int i = 0; i < stageObject.Count; i++)
        {
            if (stageObject[i].GetComponent<StagePush>().playerpush)
            {
                //踏んでいた場合番号を取得
                stageNumber = i;
            }
        }

        if (Input.GetKeyDown(KeyCode.Return))
        {
            flag.Botton();

            SceneManager.LoadScene(scenes[stageNumber]);
        }
    }

    void StageOpen()
    {
        
    }
}
