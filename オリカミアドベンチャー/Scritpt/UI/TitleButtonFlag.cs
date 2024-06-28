using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using static Unity.IO.LowLevel.Unsafe.AsyncReadManagerMetrics;

public class TitleButtonFlag : MonoBehaviour
{
    public GameObject image;
    public Image m_DrawTex;

    public bool DontDestroyEnabled = true;

    float time = 0;

    private void Start()
    {
        DontDestroyOnLoad(gameObject);
    }

    private void Update()
    {
        if (DontDestroyEnabled) return;

        time += Time.deltaTime;

        Time.timeScale = 1.0f;

        if (time > 2.0f) SceneManager.MoveGameObjectToScene(gameObject,SceneManager.GetActiveScene());
    }

    public void Botton()
    {
        StartCoroutine("Capture");
    }

    public void PlayButton()
    {
        StartCoroutine("PlayCapture");
    }

    IEnumerator Capture()
    {
        //ReadPicxels�����̌�łȂ��Ǝg���Ȃ��̂ŕK������
        yield return new WaitForEndOfFrame();

        //�X�N���[���̑傫����Sprite�����
        var texture = new Texture2D(Screen.width, Screen.height);

        //�X�N���[�����擾����
        texture.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
        //�K������
        texture.Apply();

        //�擾�����摜��Sprite�ɓ���悤�ɕϊ�����
        byte[] pngdata = texture.EncodeToPNG();
        texture.LoadImage(pngdata);

        //��قǍ����Sprite�ɉ摜�������
        Sprite sprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), new Vector2(0.5f, 0.5f));
        Debug.Log("c");

        //Sprite���g�p����I�u�W�F�N�g�Ɏw�肷��
        //     �����UI��Image
        m_DrawTex.GetComponent<Image>().sprite = sprite;

        // �T�C�Y�ύX
        m_DrawTex.GetComponent<RectTransform>().sizeDelta = new Vector2(texture.width, texture.height);

        //image���A�N�e�B�u�ɂ���
        image.SetActive(true);

        nextScene();
    }

    IEnumerator PlayCapture()
    {
        //ReadPicxels�����̌�łȂ��Ǝg���Ȃ��̂ŕK������
        yield return new WaitForEndOfFrame();

        //�X�N���[���̑傫����Sprite�����
        var texture = new Texture2D(Screen.width, Screen.height);

        //�X�N���[�����擾����
        texture.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
        //�K������
        texture.Apply();

        //�擾�����摜��Sprite�ɓ���悤�ɕϊ�����
        byte[] pngdata = texture.EncodeToPNG();
        texture.LoadImage(pngdata);

        //��قǍ����Sprite�ɉ摜�������
        Sprite sprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), new Vector2(0.5f, 0.5f));
        Debug.Log("c");

        //Sprite���g�p����I�u�W�F�N�g�Ɏw�肷��
        //     �����UI��Image
        m_DrawTex.GetComponent<Image>().sprite = sprite;

        // �T�C�Y�ύX
        m_DrawTex.GetComponent<RectTransform>().sizeDelta = new Vector2(texture.width, texture.height);

        //image���A�N�e�B�u�ɂ���
        image.SetActive(true);
    }

    public void nextScene()
    {
        //destroy�ł���悤�ɂ���
        DontDestroyEnabled = false;
    }

    public void StageSelectScene()
    {
        Time.timeScale = 1.0f;

        SceneManager.LoadScene("StageSelect");
    }

    //�I���{�^��
    public void ExitButton()
    {
        Application.Quit();
    }
}
