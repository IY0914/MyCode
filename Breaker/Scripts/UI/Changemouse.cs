//Cursor.SetCursor(handCursor, Vector2.zero, CursorMode.Auto);
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement; // コレ重要

public class Changemouse : MonoBehaviour
{
    public Texture2D handCursor;
    public Texture2D handClickCursor;
    void Start()
    {
        //Cursor.SetCursor(画像,カーソルの基準位置(new Vector2(画像の半分の大きさ)),カーソルの描画方法(autoはカーソルの大きさ参照,ForceSoftWereは画像の大きさ参照))
        Cursor.SetCursor(handCursor, new Vector2(78, 78), CursorMode.ForceSoftware);
    }
    void Update()
    {
        if (SceneManager.GetActiveScene().name == "TitleScene")
        { 
            if (Input.GetMouseButton(0))
            {
                Cursor.SetCursor(handClickCursor, new Vector2(78, 78), CursorMode.ForceSoftware);
                //クリックしたら音再生
                GetComponent<AudioSource>().Play();
            }
            else
            {
                Cursor.SetCursor(handCursor, new Vector2(78, 78), CursorMode.ForceSoftware);
            }
        }
        else
        {
            //mouseカーソルをデフォルトに変更
            Cursor.SetCursor(null, Vector2.zero, CursorMode.ForceSoftware);
        }
    }
}
